open Serbet.Endpoint;
open Async;
open Belt;

Dotenv.config();
[@bs.val] external useHttps: string = "process.env.USE_HTTPS";

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
  let endpoint =
    Serbet.endpoint({
      verb: GET,
      path: "/art/:tokenId",
      handler: req => {
        let%Async params = req.requireParams(tokenUriParams_decode);

        // Because jason is silly and deployed token 0 with 1, and 1 with 2, classic off by one!
        let tokenId =
          switch (params.tokenId) {
          | "1" => "0"
          | "2" => "1"
          | id => id
          };

        let optArtwork = GalleryArtState.runningState->Js.Dict.get(tokenId);

        (
          switch (optArtwork) {
          | Some({id, address, imageUrl}) =>
            OkJson(
              {
                description: "This gallery space is always for sale!",
                name: {j|Always for sale gallery space #$id|j},
                image: imageUrl,
                attributes: {
                  tokenId: id,
                  tokenAddress: address,
                },
              }
              ->body_out_encode,
            )
          | None => NotFound("Couldn't find a token of that id")
          }
        )
        ->async;
      },
    });
};

let app = Serbet.application(~port=5000, [TokenUri.endpoint]);

if (useHttps->String.uppercase_ascii == "TRUE") {
  Https.setupHttps(. app.expressApp)->ignore;
} else {
  ();
};

GalleryArtState.runStateWatcher();
