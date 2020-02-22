// Quick javascript hack to get https working.
let setupHttps: (. Express.App.t) => unit = [%raw
  {|
    app => {
      const fs = require("fs");
      const https = require("https");

      const privateKey = fs.readFileSync(
        "/etc/letsencrypt/live/wildcards.xyz/privkey.pem",
        "utf8"
      );
      const certificate = fs.readFileSync(
        "/etc/letsencrypt/live/wildcards.xyz/cert.pem",
        "utf8"
      );
      const ca = fs.readFileSync(
        "/etc/letsencrypt/live/wildcards.xyz/chain.pem",
        "utf8"
      );

      const credentials = {
        key: privateKey,
        cert: certificate,
        ca: ca
      };
      const httpsServer = https.createServer(credentials, app);
      httpsServer.listen(443, () => {
        console.log("HTTPS Server running on port 443");
      });
    }
|}
];
