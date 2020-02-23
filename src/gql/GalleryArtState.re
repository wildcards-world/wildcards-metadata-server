open Belt;
[%raw "require('isomorphic-fetch')"];

module Artworks = [%graphql
  {|
    subscription {
      artWorks(first: 5) {
        id
        tokenAddress
        tokenId
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
// TODO: this could probably use decco instead!
external dangerousConversion: Js.Json.t => artworks = "%identity";

[@decco.decode]
type openSeaAsset = {image_url: string};

let runStateWatcher = () => {
  let stateChangeSubscription = Artworks.make();
  let stateChangeSubscriptionMade = Gql.gql(. stateChangeSubscription##query);

  Gql.makeQuery(
    stateChangeSubscriptionMade,
    None,
    (. json) => {
      let artworks = dangerousConversion(json);

      (
        artworks.artWorks
        |> Js.Array.map(artwork => {
             let {id, tokenId, tokenAddress} = artwork;
             Js.log(
               {j|https://api.opensea.io/api/v1/asset/$tokenAddress/$tokenId/|j},
             );
             Js.Promise.(
               Fetch.fetch(
                 {j|https://api.opensea.io/api/v1/asset/$tokenAddress/$tokenId/|j},
               )
               |> then_(Fetch.Response.json)
               //  |> then_(json => openSeaAsset_decode)
               |> then_(json => {json->openSeaAsset_decode |> resolve})
               |> then_(imgUrlResult => {
                    (
                      switch (imgUrlResult) {
                      | Result.Ok(result) => result.image_url
                      | Result.Error(error) => "cannot load image (TODO: put a default image)"
                      }
                    )
                    |> resolve
                  })
               |> then_(imageUrl => {
                    runningState
                    ->Js.Dict.set(
                        id,
                        {id: tokenId, address: tokenAddress, imageUrl},
                      )
                    ->ignore;
                    Fetch.fetch(
                      {j|https://api.opensea.io/api/v1/asset/0x09edf208c44952F90Bc7670C6F3c6c8BCFFb7AD0/$id/?force_update=true|j},
                    )
                    ->ignore;
                    () |> resolve;
                  })
             )
             //  |> then_(Fetch.Response.json)
             ->ignore;
           })
      )
      ->ignore;
    },
  )
  ->ignore;
};
