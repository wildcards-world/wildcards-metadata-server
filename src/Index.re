open Serbet.Endpoint;
open Async;

module TokenUri = {
  [@decco.decode]
  type tokenUriParams = {tokenId: string};
  [@decco.encode]
  type attributes = {
    tokenId: string,
    tokenAddress: string,
  };
  [@decco.encode]
  type body_out = {
    description: string,
    name: string,
    image: string,
    attributes,
  };

  let wildcardsEndpoint =
    Serbet.endpoint({
      verb: GET,
      path: "/token/:tokenId",
      handler: req => {
        let%Async params = req.requireParams(tokenUriParams_decode);

        let tokenId = params.tokenId;
        let optWildcardMetadata = WildcardMetadata.getMetadata(tokenId);

        (
          switch (optWildcardMetadata) {
          | Some(wildcardMetadata) =>
            OkJson(wildcardMetadata->WildcardMetadata.wildcardMetaData_encode)
          | None => NotFound("Couldn't find a token of that id")
          }
        )
        ->async;
      },
    });
};
module ThreeBox = {
  [@decco.decode]
  type body_in = {
    did: string,
    twitterHandle: string,
  };
  [@decco.encode]
  type body_out = {
    success: bool,
    error: option(string),
  };
  let verification3boxTwitter =
    Serbet.jsonEndpoint({
      verb: POST,
      path: "/verification3boxTwitter",
      body_in_decode,
      body_out_encode,
      handler: ({did, twitterHandle}, _req) => {
        let%Async verificationResult =
          ThreeBoxVerification.makeTwitter3boxVerification(
            ~did,
            ~twitterHandle,
          );
        Js.log2("verification result", verificationResult);
        (
          switch (verificationResult) {
          | Ok(result) =>
            switch (result) {
            | {status: "success"} => {success: true, error: None}
            | {status: "error", message, data: _} => {
                success: false,
                error: message,
              }
            | {message} => {success: false, error: message}
            }
          | Error(error) => {
              success: false,
              error:
                Some(
                  "CRITICAL decco ERROR: at: "
                  ++ error.path
                  ++ "with message: "
                  ++ error.message,
                ),
            }
          }
        )
        ->async;
      },
    });
};

let app =
  Serbet.application(
    ~port=5000,
    [
      TokenUri.wildcardsEndpoint,
      ThreeBox.verification3boxTwitter,
    ],
  );

