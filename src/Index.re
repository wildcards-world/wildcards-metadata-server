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

        let optArtwork =
          GalleryArtState.runningState->Js.Dict.get(params.tokenId);

        (
          switch (optArtwork) {
          | Some(artwork) =>
            OkJson(
              {
                description: "This gallery space is always for sale!",
                name: "Always for sale gallery space 1",
                image: artwork.imageUrl,
                attributes: {
                  tokenId: artwork.id,
                  tokenAddress: artwork.address,
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

Js.log(useHttps);
Js.log(useHttps->String.uppercase_ascii);
if (useHttps->String.uppercase_ascii == "TRUE") {
  Js.log("true");
  Https.setupHttps(. app.expressApp)->ignore;
} else {
  Js.log("false");
  ();
};

// GalleryArtState.runStateWatcher();
