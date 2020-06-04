[@decco.encode]
type wildcardMetaData = {
  artist: string,
  name: string,
  image: string,
  _type: string,
  commonName: string,
  affiliation: string,
  description: string,
};

let metadata: Js.Dict.t(wildcardMetaData) = [%raw
  {|
  {
    8: {
      artist: "Funi Mathonsi",
      name: "Espumita",
      image: "https://ipfs.io/ipfs/QmU1y8HxrYcUDoAUF2A6yLmSTDnPwVL137Vri4GbRcNKVx",
      type: "Puma concolor",
      commonName: "Puma",
      affiliation: "Senda Verde",
      description: "Espumita has been with La Senda Verde for just over two years. She came as a small kitten from the authorities in La Paz who had confiscated her from a poacher. At the time La Senda Verde didn't have anywhere suitable to accommodate her and as a small kitten she spent her first few months in the clinic. La Senda Verde raised funds and built her a management cage whilst she was still growing. Then, at the beginning of 2017 they completed her full sized enclosure with high platforms, on a hill so she can get to higher ground and with a den at the top that she loves to sit in and watch out from.\n\nEspumita has always been quite a shy and easily stressed cat but loves her new home. In December Espumita had unfortunately been under stress and was pacing a lot due to recent construction works going on close to her enclosure. Unfortunately, Espumita also wasn't eating very well and had lost condition. Eight weeks ago, la Senda Verde sedated her and a visiting group of vets studying ultrasonography came and performed an ultrasound on her and we were able to give her a thorough check. It was found that she likely had some mild hepatic lipidosis (a form of liver disease) which was also contributing to her loss of appetite.\n\nAlong with one of the sanctuary staff members, Vicky from Senda Verde started to add in extra enrichment every day for Espumita and slightly change the time and way in which she was fed. Luckily, at this time the construction was also finished. Over the next eight weeks there was an amazing transformation! Espumita started to eat normally, gain weight and is noticeably more relaxed and confident. She comes down to greet the staff from la Senda Verde every time they go for enrichment or cleaning and feeding (which she never did before) and Senda Verde hasn't seen her do any pacing that she was doing before.\n\nShe absolutely loves cardboard boxes of any shape or size! La Senda Verde started off with relaxing smells such as chamomile flowers and lavender in cardboard boxes. They have also been using feathers which she loves. She is still a huge fan of lavender and will roll in it and will then often have a mad ten minutes running all over her enclosure with the box and up on her platforms having so much fun."
    },
    13: {
      artist: "Denham Preen",
      name: "Glen the Dragon from Turin",
      image: "https://ipfs.io/ipfs/QmZaJQBSezhg1Gya2LEduvtzRSk9Jgek6Ay5iqhz4oB3Qx",
      type: "Mythical Dragon",
      commonName: "Dragon",
      affiliation: "governance",
      description: "It was a tough and trying time in Turin, Italy. A pandemic had taken over the land and the villagers were facing hardships. Unexpectedly nature was prospering, the villagers were in hiding and the flora and fauna of the region was reclaiming its land. Most unexpectedly the dragon returned, which for a long time was believed to be extinct. Glen the Dragon of Turin emerged from the Alps just North of the city. One of the last of his kind.\n\nGlen is a unique Wildcard that is named after Glen Weyl the author of Radical Markets. He was developed during the ETHTurin hackathon. The funds raised by Glen go to the conservation voted by the community. Owners of wildcards can vote using their loyalty tokens using quadratic voting to vote which conservation should receive the funds raised for that month."
    },
    14:{
      artist: "?",
      name: "Ucock",
      image: "https://ipfs.io/ipfs/QmWWHN5BMumoGooSzTfRznEjnp5qRi2Upyx2gc6rMPnn26",
      type: "Helarctos malayanus",
      commonName: "Sun bear",
      affiliation: "Darwin Animal Doctors",
      description: "Ucok is a baby sun bear who was taken way too young from his mom, and sold into wildlife trafficking. However, Darwin Animal Doctors local partner team, the Indonesian Species Conservation Program (ISCP) rescued him from the black market. Ucok is now being rehabilitated at the BKSDA - the Indonesian wildlife authority. Ucok is understandably often quite sad, because he misses his mom. But, he regularly overcomes his sadness to be an extremely curious and inquisitive boy and he loves to test his rehab team's boundaries! He is quite mischievous, which tells us he's a smart boy!"
    },
    15:{
      artist: "Funi Mathonsi",
      name: "Tarkus",
      image: "https://ipfs.io/ipfs/QmQSDdGitMjpdES2bGeKYvDRpvzmBu6LcLB7diXGyWtcY5",
      type: "Tremarctos ornatus",
      commonName: "Andean bear",
      affiliation: "Senda Verde",
      description: "Tarkus was found on the roadside by a trucker in a section between Sucre and Tarija where Andean bears are known to exist. A female bear got scared by the truck and ran with the two cubs. The smallest one fell a bit behind. The trucker decided to stop, capture the cub and take him to his house, where he kept him for more than two weeks. Later he called the environmental authorities in the City of Tarija to turn him in. Capturing the cub is never a good idea, it is virtually condemning him to a life in captivity for he will never be able to learn the abilities to survive in the wild his mother would have taught him. If he would have been left alone, the mother would have come back for him later.\n\nTarkus stayed in Tarija for three weeks while the formal requirements were fulfilled. National authorities advised he should be sent to La Senda Verde, where there is a place for the cub, but also the experience of raising bear cubs with appropriate diet and management. He has been placed in a 6x5x6 enclosure, just for him, and he has been getting used to the refuge and the caretakers.\n\nTarkus has adapted well to the refuge and loves to play with the ball and in the water."
    },
    16:{
      artist: "BruceTheGoose",
      name: "Hook",
      image: "https://ipfs.io/ipfs/QmUC8V7Pr5FzdbkHYB22PyAU7wLdUDvAqbBQ2NdAzxrTJe",
      type: "Balaenoptera musculus",
      commonName: "Blue Whale",
      affiliation: "The Great Whale Conservancy",
      description: "Hook is a male blue whale that the Great Whale Conservancy has been keeping track of for a long time,close to 40 years. They have seen and photographically identified Hook for the past 6 years in the row which is a record siting for the Great Whale Conservancy. Hook has quite a dark pigmentation for a blue whale and often has a large number of remoras attached to his body underneath the dorsal fin.\n\nHook is often one of the first whales to arrive at the Sea of Cortez feeding grounds in the Great Whale Conservancies working area in the general vicinity of Loreto, Baja California. He is one of the finest examples of 'habitat preference' coming back to the same part of the sea for many years in a row and a significant number over the past 38 years."
    },
    17: {
      artist: "Funi Mathonsi",
      name: "Mijungla",
      image: "https://ipfs.io/ipfs/QmSvEbEbwPkRzwPPjisBqL4W4Gm93wUhaccZ1XQJaVsWM8",
      type: "Panthera onca",
      commonName: "Jaguar",
      affiliation: "Senda Verde",
      description: "Mijungla arrived December 13th to La Senda Verde to be the second jaguar to live in the animal refuge. She was picked up from a family in the town of Palos Blancos, located in a low tropical rainforest area where the Amazon meets the Andes.\n\nThe family has a cattle ranch, and one of the cowboys in the ranch had found her in a very bad state of health laying in the forest, with a wound in her head, full of skin parasites and parts of her body without skin. They picked her up and started rehabilitating her. First they fed her the wrong food, then they realised they needed to feed her meat. It wasn't until she arrived with Senda Verde that she was able to drink cat milk, for she was so small she really needed it.\n\nThe cub was partially rehabilitated by the family but still had some difficulties when she arrived at Senda Verde. She couldn't walk very well and her stomach was swollen. Just like the Andean Bear cub Tarkus that arrived around the same time Mijungla did, this Jaguar cub spent very little time with her mother, which affects them greatly both psychologically and physiologically. Senda Verde monitors very closely both cub's digestion since they could get sick since they did not get enough milk from their mothers.\n\nWhen he arrived, Mijungla was three months old and weighted 3.2 kilograms."
    },
    18:{
      artist: "Funi Mathonsi",
      name: "Ajayu",
      image: "https://ipfs.io/ipfs/QmbERbhsrkLgX6kB3FnCXGeuN1WnvHmZHpp6etgypcfobz",
      type: "Tremarctos ornatus",
      commonName: "Andean bear",
      affiliation: "Senda Verde",
      description: "Ajayu was saved by the police in Cochabamba from being brutally beaten to death by a local nearby community in 2016. He arrived in La Paz and La Senda Verde took care of his treatments. He received several surgeries, one to save his left eye and one to save his jaw from a severe infection. He stopped eating for days and had to be force fed. It was a struggle to save him, but with time he got better and now weighs 100 kilograms.\n\nAs a result from this brutal story was that la Senda Verde has become more involved in trying to change legislation in Bolivia to protect this species and avoid stories like this one from happening again. They are now working alongside biologist Ximena Velez-Liendo to present a law project that will be called the Ajayu law, named after this bear. Ajayu means spirit in Aymara language."
    }
  }
|}
];

let getMetadata = tokenId => {
  metadata->Js.Dict.get(tokenId);
};