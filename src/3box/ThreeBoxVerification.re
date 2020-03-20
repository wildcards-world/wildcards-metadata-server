[%raw "require('isomorphic-fetch')"];

open Globals;

type profile = {
  description: option(string),
  name: option(string),
  proof_twitter: option(string),
  website: option(string),
  location: option(string),
  proof_did: option(string),
  memberSince: option(string),
};
type twitterVerification = {
  username: option(string),
  proof: option(string),
  verifiedBy: option(string),
};
type verifications = {
  did: option(string),
  muport: option(string),
  twitter: option(twitterVerification),
};

[@decco.decode]
type verificationData = {verification: string};
// NOTE: this could be improved by: https://github.com/reasonml-labs/decco/issues/47
//                                  or possibly (#36 / #39)
[@decco.decode]
type twitterVerificationResult = {
  status: string,
  data: option(verificationData),
  message: option(string),
};

[@bs.module "3box/lib/api"] [@bs.val]
external getProfile: (. string) => Js.Promise.t(profile) = "getProfile";
[@bs.module "3box/lib/api"] [@bs.val]
external getVerifiedAccounts: (. profile) => Js.Promise.t(verifications) =
  "getVerifiedAccounts";

let createTwitterVerificationError = message =>
  {status: "error", data: None, message: Some(message)}->Obj.magic;
let errorCatcherHelper = errorStringPrefix =>
  catchAsync(_, err =>
    Js.Promise.resolve(
      createTwitterVerificationError(errorStringPrefix ++ err->Obj.magic),
    )
  );
let makeTwitter3boxVerification = (~did, ~twitterHandle) =>
  Fetch.(
    {
      let%Async response =
        fetchWithInit(
          "https://verifications.3box.io/twitter/",
          RequestInit.make(
            ~method_=Post,
            ~body=
              BodyInit.make(
                {j|{"did": "$did", "twitter_handle": "$twitterHandle"}|j},
              ),
            ~headers=
              HeadersInit.make({
                "Content-Type": "text/plain;charset=UTF-8",
                "Origin": "https://3box.io",
              }),
            (),
          ),
        )
        |> errorCatcherHelper("3box FETCH ERROR: ");

      let%Async responseJson =
        if (response->Response.ok) {
          response->Response.json
          |> errorCatcherHelper("3box FETCH JSON DECODE ERROR: ");
        } else {
          Js.Promise.resolve(
            createTwitterVerificationError(
              "3box FETCH ERROR: server returned invalid response",
            ),
          );
        };

      // TODO: probably worth catching errors here...
      responseJson->twitterVerificationResult_decode->async;
    }
  );

let getTwitterHandle3box = ethAddress => {
  let%Async profile = getProfile(. ethAddress);
  let%Async verifiedAccounts = getVerifiedAccounts(. profile);
  (verifiedAccounts.twitter >>= (twitter => twitter.username))->async;
};
