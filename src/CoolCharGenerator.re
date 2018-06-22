let str = ReasonReact.string;

Random.self_init();

type coolChar = {
  text: string,
  caption: string,
}

/* State declaration */
type state = {
  chars: array(coolChar),
  mode: string,
};

/* Action declaration */
type action =
  | AddChar
  | Clear
  | ChangeMode(string);

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("CoolCharGenerator");

let getMode = (mode) =>
  if (mode == "Either") {
    switch (Random.int(2)) {
    | 0 => "Hanzi"
    | _ => "Emoji"
    }
  } else
    mode;

let getCoolChar = mode => {
  let mode_ = getMode(mode);
  if (mode_ == "Hanzi") {
    let hanzi = Hanzi.getHanzi();
    {
      text: hanzi.text,
      caption: Printf.sprintf("Code point: %d", hanzi.ordinal),
    }
  } else {
    let emoji = Emoji.getEmoji();
    {
      text: emoji.text,
      caption: Printf.sprintf("%s (%s)", emoji.shortname, emoji.category),
    }
  }
};

/* greeting and children are props. `children` isn't used, therefore ignored.
   We ignore it by prepending it with an underscore */
let make = (_children) => {
  /* spread the other default fields of component here and override a few */
  ...component,

  initialState: () => {
    chars: [||],
    mode: "Either"
  },

  didMount: self => self.send(AddChar),

  /* State transitions */
  reducer: (action, state) => ReasonReact.(
    switch (action) {
      | AddChar => Update({
            ...state,
            chars: Array.append(state.chars, [|getCoolChar(state.mode)|])
          })
      | Clear => Update({...state, chars: [||]})
      | ChangeMode(mode) => Update({...state, mode: mode})
    }
  ),

  render: ({state, send}) => {
    let changeModeOption = (label) =>
      <option value=label>
        (str(label))
      </option>;

    <div>
      <div className="form-inline">
        <select className="form-control mr-2"
                value=state.mode
                onChange=(evt =>
                  send(
                    ChangeMode(
                      (
                        evt
                        |. ReactEventRe.Form.target
                        |. ReactDOMRe.domElementToObj
                      )##value
                    )
                  )
                )>
          (changeModeOption("Hanzi"))
          (changeModeOption("Emoji"))
          (changeModeOption("Either"))
        </select>
        <button className="btn btn-primary btn-sm mr-2"
                onClick=(_ => send(AddChar))>
          (str("Generate"))
        </button>
        <button className="btn btn-primary btn-sm"
                onClick=(_ => send(Clear))>
          (str("Clear"))
        </button>
      </div>
      <div className="chars">
        (
          state.chars
          |> Array.mapi((i, cc) =>
              <span key=string_of_int(i) title=cc.caption>
                (str(cc.text))
              </span>)
          |> ReasonReact.array
        )
      </div>
    </div>;
  },
};