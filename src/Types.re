[@decco.decode]
type tokenUriParams = {tokenId: string};

[@decco.encode]
type body_out = {
  name: option(string),
  image: option(string),
  description: string,
  artist: option(string),
  artistWebsite: option(string),
  organisation: option(string),
  organisationWebsite: option(string),
  species: option(string)
};
