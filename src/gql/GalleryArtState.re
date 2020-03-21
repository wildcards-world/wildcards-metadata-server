open Belt;
[%raw "require('isomorphic-fetch')"];

module Artworks = [%graphql
  {|
    query {
      artWorks(first: 10) {
        id
        tokenAddress
        tokenId
      }
    }
  |}
];

module LatestArtwork = [%graphql
  {|
    subscription {
      lastChangedArtwork(id: "0") {
        artSpotId
        count
        tokenAddress
        tokenId
      }
    }
  |}
];
module LatestArtChangeCount = [%graphql
  {|
    query {
      lastChangedArtwork(id: "0") {
        count
      }
    }
  |}
];

type imageDetails = {
  id: string,
  address: string,
  imageUrl: string,
};
let runningState: Js.Dict.t(imageDetails) = Js.Dict.empty();
// let runningState: ref(Js.Dict.t(imageDetails)) = ref(Js.Dict.empty());

type artwork = {
  id: string,
  tokenAddress: string,
  tokenId: string,
};
type artworks = {artWorks: array(artwork)};
type lastChangedArtwork = {lastChangedArtwork: artwork};
type countObj = {count: string};

// TODO: this should  use decco instead!
let dangerousConversion: Js.Json.t => artworks = Obj.magic;
let dangerousGetCount: Js.Json.t => countObj = Obj.magic;

[@decco.decode]
type openSeaAsset = {image_url: string};

let getArtworkDetails = artwork => {
  let {id, tokenId, tokenAddress} = artwork;
  Js.log(
    {j|Fetching the artwork details now for $id!! - https://api.opensea.io/api/v1/asset/$tokenAddress/$tokenId/|j},
  );
  Js.Promise.(
    Fetch.fetch(
      {j|https://api.opensea.io/api/v1/asset/$tokenAddress/$tokenId/|j},
    )
    |> then_(Fetch.Response.json)
    |> then_(json => {json->openSeaAsset_decode |> resolve})
    |> then_(imgUrlResult => {
         (
           switch (imgUrlResult) {
           | Result.Ok(result) => result.image_url
           | Result.Error(error) =>
             Js.log(error);
             "cannot load image (TODO: put a default image)";
           }
         )
         |> resolve
       })
    |> then_(imageUrl => {
         runningState
         ->Js.Dict.set(id, {id: tokenId, address: tokenAddress, imageUrl})
         ->ignore;

         (
           Fetch.fetch(
             {j|https://api.opensea.io/api/v1/asset/0x09edf208c44952F90Bc7670C6F3c6c8BCFFb7AD0/$id/?force_update=true|j},
           )
           |> then_(Fetch.Response.json)
           |> then_(json => {
                // TODO: check that the result is correct (ie it did infact refresh)
                //       An additional check can be done with the `/validate/` extension. The "errors" array should be empty.
                let result = json->openSeaAsset_decode;
                result |> resolve;
              })
         )
         ->ignore;
         () |> resolve;
       })
  )
  ->ignore;
};

let artChangeCount = ref("NOT_SET_YET");

// I have separated this so I can easily restart this function!
let startArtUpdateSubscription = artChangeSubscriptionMade =>
  Gql.makeQuery(
    artChangeSubscriptionMade,
    None,
    (. json) => {
      artChangeCount := dangerousGetCount(json).count;
      Js.log(json);
      let artwork = [%raw "{...json, id: json.artSpotId}"];

      artwork.lastChangedArtwork->getArtworkDetails->ignore;
    },
  )
  ->ignore;

let runStateWatcher = () => {
  let getInitialArtworksQuery = Artworks.make();
  let getInitialArtworksQueryMade = Gql.gql(. getInitialArtworksQuery##query);

  Gql.makeQuery(
    getInitialArtworksQueryMade,
    None,
    (. json) => {
      let artworks = dangerousConversion(json);

      (artworks.artWorks |> Js.Array.map(getArtworkDetails))->ignore;
    },
  )
  ->ignore;

  let artChangeSubscription = LatestArtwork.make();
  let artChangeSubscriptionMade = Gql.gql(. artChangeSubscription##query);
  startArtUpdateSubscription(artChangeSubscriptionMade);

  let getArtChangeCountQuery = LatestArtChangeCount.make();
  let getArtChangeCountQueryMade = Gql.gql(. getArtChangeCountQuery##query);

  Js.Global.setInterval(
    () => {
      Gql.makeQuery(
        getArtChangeCountQueryMade,
        None,
        (. json) => {
          let count = dangerousGetCount(json).count;
          if (count != artChangeCount^) {
            Js.log("The count was out of sync! Restarting the subscription.");
            startArtUpdateSubscription(artChangeSubscriptionMade);
          } else {
            ();
          };
        },
      )
      ->ignore
    },
    6000,
  );
};
