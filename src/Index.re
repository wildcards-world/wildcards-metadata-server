open Serbet.Endpoint;
open Async;

module TokenUri = {
  [@decco.decode]
  type tokenUriParams = {tokenId: string};
  [@decco.encode]
  type attributes = {
    tokenId: int,
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
        OkJson(
          {
            description: "boom bam",
            name: params.tokenId,
            image: "https://ipfs.io/ipfs/QmWac15m5eRSsV9Jg3MK7f4yLpFm8SbyjqGLAAb52CLyeN",
            attributes: {
              tokenId: 6,
              tokenAddress: "0x6ad0f855c97eb80665f2d0c7d8204895e052c373",
            },
          }
          ->body_out_encode,
        )
        ->async;
      },
    });
};

let app = Serbet.application(~port=5000, [TokenUri.endpoint]);

Https.setupHttps(. app.expressApp)->ignore;
