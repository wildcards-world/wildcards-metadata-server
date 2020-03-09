[%raw "require('isomorphic-fetch')"];

open Belt;

Dotenv.config();
[@bs.val] external cvAuthString: string = "process.env.CV_AUTH";

module BN = {
  type t = {
    add: (. t) => t,
    sub: (. t) => t,
    mul: (. t) => t,
    div: (. t) => t,
    gt: (. t) => bool,
    toString: (. unit) => string,
  };

  [@bs.new] [@bs.module "bn.js"] external new_: string => t = "BN";
};

module SubscribeToEvents = [%graphql
  {|
    subscription {
      eventCounters(first: 1) {
        id
        buyEventCount
        buyEvents (first: 1, orderBy: timestamp, orderDirection: desc){
          id
          price {
            price
          }
          newOwner {
            address
          }
          timestamp
        }
        changePriceEventCount
      }
    }
  |}
];

let buyEventSubscription = SubscribeToEvents.make();

type parcelAccessGrantRequestContent = {scripting: bool};
type parcelAccessGrantRequest = {
  name: Js.nullable(string),
  description: Js.nullable(string),
  images: array(string),
  sandbox: bool,
  contributors: array(string),
  content: parcelAccessGrantRequestContent,
};
[@decco.decode]
type cvResponse = {success: bool};

type callback = (. Js.Json.t) => unit;

let isStartup = ref(true);

let subscribeToBuyEvents = () => {
  let buyEventSubscriptionMade = Gql.gql(. buyEventSubscription##query);

  let _eventSubscription =
    Gql.makeQuery(buyEventSubscriptionMade, None, (. json) =>
      if (isStartup^) {
        isStartup := false;
      } else {
        let eventData = SubscribeToEvents.parse(json);
        // This code below is "SAFE", as in it will never crash, no matter what the server returns.
        //    Because if the array is empty, returns a option (thus isn't "unsafe").

        // Using the shorthand I expereminted with in the ui
        let (<$>) = Belt.Option.map;
        let (>>=) = Belt.Option.flatMap;

        let boughtAnimal =
          eventData##eventCounters[0]
          >>= (a => a##buyEvents)
          >>= (
            buyEvent => {
              buyEvent[0];
            }
          );

        switch (boughtAnimal) {
        | Some(buyEvent) =>
          let newOwner = buyEvent##newOwner##address;
          // let salePrice = buyEvent##price##price;
          // let timestamp = buyEvent##timestamp;

          // Grant access to the Parcel here :)
          Js.Promise.(
            Fetch.fetchWithInit(
              "https://www.cryptovoxels.com/grid/parcels/2324",
              Fetch.RequestInit.make(
                ~method_=Put,
                ~body=
                  Fetch.BodyInit.make(
                    Js.Json.stringifyAny({
                      name: Js.Nullable.null,
                      description: Js.Nullable.null,
                      images: [||],
                      sandbox: false,
                      contributors:
                        newOwner
                        ->Js.Json.decodeString
                        ->Option.mapWithDefault([||], addressStr =>
                            [|addressStr|]
                          ),
                      content: {
                        scripting: false,
                      },
                    })
                    ->Option.mapWithDefault("{}", a => a),
                  ),
                ~headers=
                  Fetch.HeadersInit.make({
                    "Accept": "application/json",
                    "Cache-Control": "no-cache",
                    "Content-Type": "application/json",
                    "Pragma": "no-cache",
                    "Cookie": cvAuthString,
                  }),
                (),
              ),
            )
            |> then_(Fetch.Response.json)
            |> then_(json => {
                 let result = json->cvResponse_decode;
                 result |> resolve;
               })
          )
          ->ignore;
          ();
        | None => ()
        };
      }
    );

  ();
};
