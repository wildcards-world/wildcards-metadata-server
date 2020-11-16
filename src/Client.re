[%raw "require('isomorphic-fetch')"];
[@bs.val]
external fetch: ApolloClient.Link.HttpLink.HttpOptions.Js_.t_fetch = "fetch";

let graphqlEndpoint = "https://api.wildcards.world/v1/graphql";

let httpLink =
  ApolloClient.Link.HttpLink.make(
    ~uri=_ => graphqlEndpoint,
    (),
  );

let instance =
  ApolloClient.(
    make(
      ~cache=Cache.InMemoryCache.make(),
      ~connectToDevTools=true,
      ~defaultOptions=
        DefaultOptions.make(
          ~mutate=
            DefaultMutateOptions.make(
              ~awaitRefetchQueries=true,
              ~errorPolicy=All,
              (),
            ),
          ~query=
            DefaultQueryOptions.make(
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          ~watchQuery=
            DefaultWatchQueryOptions.make(
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          (),
        ),
      ~link=httpLink,
      (),
    )
  );
