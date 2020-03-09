// Remove the apollo-boost import and change to this:
let fetch = require("node-fetch").default;
let ApolloClient = require("apollo-client").default;

// Setup the network "links"
const { SubscriptionClient } = require("subscriptions-transport-ws");
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
const subscriptionClient = new SubscriptionClient(
  "wss://api.thegraph.com/subgraphs/name/jasoons/always-for-sale-gallery", // use wss for a secure endpoint
  {
    timeout: 30000,
    lazy: false,
    reconnectionAttempts: 10,
    connectionCallback: error => {
      console.log("connected to socket", { error });
    },
    inactivityTimeout: 500,
    reconnect: true
  },
  ws
);
const wsLink = new WebSocketLink(subscriptionClient);

// For analysis purposes...
subscriptionClient.onConnected(() => console.log("WS -- Connected!!"));
subscriptionClient.onReconnected(() => console.log("WS -- RE-Connected!!"));
subscriptionClient.onConnecting(() => console.log("WS -- Connecting!!"));
subscriptionClient.onReconnecting(() => console.log("WS -- RE-Connecting!!"));
subscriptionClient.onDisconnected(() => console.log("WS -- DIS-Connecting!!"));
subscriptionClient.onError(() => console.log("WS -- Error!!"));

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
