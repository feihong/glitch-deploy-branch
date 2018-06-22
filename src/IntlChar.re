type intlChar = {
  text: string,
  writingSystem: string,
  ordinal: int,
};

[@bs.deriving jsConverter]
type writingSystem = [
  | `Hanzi
  | `Hangul
  | `Kana
  | `Devanagari
];

let getCharFromRange = (min, max) => {
  let ordinal = Util.randomIntRange(min, max);
  let text = Util.fromCodePoint(ordinal);
  (text, ordinal)
};

let getCharFromCodePoints = codePoints => {
  let ordinal = Util.chooseFromArray(codePoints);
  let text = Util.fromCodePoint(ordinal);
  (text, ordinal)
};
  

let kanaCodePoints = [%bs.raw {|
  '゠ァアィイゥウェエォオカグケゲコゴサザシジスズセダチヂッツヅテデトドナニバパヒビピフブプヘベペホムメモャヤュユョヨラリルヰヱヲンヴヵヶヷヸヹヺぁあぃいぅうぇえぉおかぐけげこごさざしじすずせだちぢっつづてでとどなにばぱひびぴふぶぷへべぺほむめもゃやゅゆょよらりるゐゑをんゔゕゖ'
|}] |. Util.stringToCodePoints;

let devanagariCodePoints = [%bs.raw {|
  'ऄअआइईउऊऋऌऍऎएऐऑऒओऔकखगघङचछजझञटठडढणतथदधनऩपफबभमयरऱलळऴवशषसहऽॐक़ख़ग़ज़ड़ढ़फ़य़ॠॡ१२३४५६७८९ॲॳॴॵॶॷॸॹॺॻॼॽॾॿ'
|}] |. Util.stringToCodePoints;
Js.log(devanagariCodePoints)

let getIntlChar = writingSys => {
  let (text, ordinal) = 
    switch (writingSys) {
    | `Hanzi => getCharFromRange(0x4e00, 0x9fff)
    /* https://en.wikipedia.org/wiki/Hangul_Syllables */  
    | `Hangul => getCharFromRange(0xAC00, 0xD7AF)
    | `Kana => getCharFromCodePoints(kanaCodePoints)
    | `Devanagari => getCharFromCodePoints(devanagariCodePoints)
  };
  let label = writingSystemToJs(writingSys);
  {text, ordinal, writingSystem: label}
};