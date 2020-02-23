// Remove the apollo-boost import and change to this:
let fetch = require("node-fetch").default;
let ApolloClient = require("apollo-client").default;

// Setup the network "links"
let { WebSocketLink } = require("apollo-link-ws");
let { HttpLink } = require("apollo-link-http");
let ws = require("ws");
let { split } = require("apollo-link");
let { getMainDefinition } = require("apollo-utilities");
let { InMemoryCache } = require("apollo-cache-inmemory");

const httpLink = new HttpLink({
  uri:
    "https://api.thegraph.com/subgraphs/name/jasoons/always-for-sale-gallery", // use https for secure endpoint
  fetch: fetch
});

// Create a WebSocket link:
const wsLink = new WebSocketLink({
  uri: "wss://api.thegraph.com/subgraphs/name/jasoons/always-for-sale-gallery", // use wss for a secure endpoint
  options: {
    reconnect: true
  },
  webSocketImpl: ws
});

// using the ability to split links, you can send data to each link
// depending on what kind of operation is being sent
const link = split(
  // split based on operation type
  ({ query }) => {
    const { kind, operation } = getMainDefinition(query);
    return kind === "OperationDefinition" && operation === "subscription";
  },
  wsLink,
  httpLink
);

// Instantiate client
const client = new ApolloClient({
  link,
  cache: new InMemoryCache()
});

const subscribe = (query, variables, callback) =>
  client.subscribe({ query, variables }).subscribe({
    next: ({ data }) => {
      callback(data);
    },
    error: e => console.error(e)
  });

module.exports = {
  subscribe
};
