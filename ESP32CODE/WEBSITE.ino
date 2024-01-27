String css() {
  return R"(
      @import url("https://fonts.googleapis.com/css2?family=Roboto:wght@700&display=swap");

      header {
        display: flex;
        margin-left: 20px;
      }
      h1 {
        align-self: center;
        color: #fff;
        font-family: Roboto;
        font-size: 40px;
        font-style: normal;
        font-weight: 700;
        line-height: normal;
        margin-left: 20px;
      }

      body {
        width: 100%;
        margin: 0;
        display: flex;
        justify-content: center;
      }

      footer {
        display: flex;
        flex-direction: row;
        gap: 11px;
        margin-left: 20px;
        margin-bottom: 10px;
      }
      .contact {
        display: flex;
      }
      .contact > a {
        color: #fff;
        font-family: Roboto;
        font-size: 14px;
        font-style: normal;
        font-weight: 700;
        line-height: normal;
        text-decoration: none;
        align-self: center;
        margin-left: 7px;
      }

      .checkbox {
        position: absolute;
        z-index: -1;
        opacity: 0;
      }
      .range_LED {
        display: flex;
        flex-direction: row;
        position: relative;
        margin-top: 20px;
        margin-bottom: 20px;
        gap: 20px;
      }

      input[type="range"] {
        width: 200px;
        -webkit-appearance: none;
        -moz-appearance: none;
        appearance: none;
        outline: none;
        height: 2px;
        background: #0f182d;
        align-self: center;
      }

      input[type="range"]::before {
        display: inline-block;
        content: " ";
        position: absolute;
        bottom: -2px;
        background: #1f253c;
        width: 7px;
        height: 23px;
        left: -6px;
      }

      input[type="range"]::after {
        display: inline-block;
        content: " ";
        position: absolute;
        bottom: -2px;
        background: #1f253c;
        width: 7px;
        height: 23px;
        left: 203px;
      }

      input[type="range"]::-webkit-slider-thumb {
        background: #2b3250;
        cursor: pointer;
        width: 7px;
        height: 23px;
        -webkit-appearance: none;
      }

      label {
        color: #fff;
        font-family: Roboto;
        font-size: 18px;
        font-style: normal;
        font-weight: 700;
        line-height: normal;
      }
      main {
        margin-left: 20px;
        flex: 1 1 auto;
      }
      .checkbox + .checkbox_label {
        display: inline-flex;
        align-items: center;
        /* user-select: none; */
      }
      .checkbox + .checkbox_label::before {
        content: "";
        display: inline-block;
        flex-shrink: 0;
        flex-grow: 0;
        margin-right: 0.5em;
        background-repeat: no-repeat;
        background-position: center center;
        background-size: 50% 50%;

        width: 25px;
        height: 25px;
        border-radius: 8px;
        background: #1f253c;
      }

      .checkbox:checked + .checkbox_label::before {
        background-color:green;
        background-repeat: no-repeat;
        background-position: center;
      }

      button {
        width: 90px;
        height: 30px;
        flex-shrink: 0;
        border-radius: 15px;
        background: #1f253c;
        border: none;
      }
      button > span {
        color: #fff;
        font-family: Roboto;
        font-size: 14px;
        font-style: normal;
        font-weight: 700;
        line-height: normal;
      }

      .Name {
        width: 365px;
      }

      .Name_Gesture {
        width: 210px;
      }

      .Num_Pin {
        width: 95px;
      }

      .Input {
        border-radius: 15px;
        border: 1px solid #000;
        background: #131b41;
        height: 30px;
        color: white;
        padding-left: 5px;
      }

      input::-webkit-outer-spin-button,
      input::-webkit-inner-spin-button {
          -webkit-appearance: none;
      }

      .inputs {
        display: flex;
        flex-wrap: wrap;
        width: 365px;
        gap: 15px 41px;
      }

      .btns {
        display: flex;
        justify-content: space-between;
        margin-top: 25px;
      }

      .Input::placeholder {
        color: #8c8c8c;
        font-family: Roboto;
        font-size: 14px;
        font-style: normal;
        font-weight: 700;
        line-height: normal;
        /* padding-left: 10px; */
      }

      .Brightness {
        display: flex;
        flex-direction: column;
        height: 100vh;
        margin: 0;
        width: 100%;
        align-items: center;
        background: #131b41;
        /* filter: brightness(0.5); */
      }

      .ADD_Checkbox {
        position: absolute;
        z-index: 1;
        background: #131b41;
        padding: 10px;
        border-radius: 10px;
        transition: all 0.5s ease-out;
        margin-top: -180px;
      }

      .checkbox_remove_btn{
        background: none;
        width: 25px;
        height: 25px;
      }

      .checkbox_remove_btn> img{
        width: 25px;
      }
      .range_LED{
        display:none;
      }

)";
}




String script() {
  return R"(



  let GestureData = '';

  const Banner_add_checkbox = document.querySelector(".ADD_Checkbox");
  const ADD_Checkbox_btn = document.querySelector(".ADD_Checkbox_btn");
  const Brightness = document.querySelector(".Brightness");

  const Checkboxs = [];

  const svgTrash = `
    <svg viewBox="0,0,256,256" width="24px" height="24px" fill-rule="nonzero">
    <g fill="#1f253c" fill-rule="nonzero" stroke="none" stroke-width="1" stroke-linecap="butt" stroke-linejoin="miter" stroke-miterlimit="10" stroke-dasharray="" stroke-dashoffset="0" font-family="none" font-weight="none" font-size="none" text-anchor="none" style="mix-blend-mode: normal">
    <g transform="scale(10.66667,10.66667) ">
    <path d="M10.3125,-0.03125c-1.72266,0 -3.14844,1.34766 -3.3125,3.03125h-5v2h4.96875v0.03125h10.0625v-0.03125h4.96875v-2h-5c-0.15625,-1.68359 -1.51562,-3.03125 -3.1875,-3.03125zM10.3125,2.03125h3.5c0.50781,0 0.88281,0.39844 1.03125,0.9375h-5.6875c0.14063,-0.53906 0.53125,-0.9375 1.15625,-0.9375zM4,6v16.5c0,0.80078 0.69922,1.5 1.5,1.5h13.09375c0.80078,0 1.5,-0.69922 1.5,-1.5v-16.5zM7,9h1v13h-1zM10,9h1v13h-1zM13,9h1v13h-1zM16,9h1v13h-1z">
    </path>
    </g></g></svg>
  `;

  const createCheckbox = (className, Text, id) => {
    const checkbox = document.createElement("div");
    const InputCheckbox = document.createElement("input");
    const LabelCheckbox = document.createElement("label");
    const RemoveBTN = document.createElement("button");
    const IMG_RemoveBTN = document.createElement("div");

    checkbox.classList.add(className);
    InputCheckbox.type = "checkbox";
    InputCheckbox.id = `${className}_id`;
    InputCheckbox.classList.add(`checkbox_${className}`);
    InputCheckbox.classList.add("checkbox");

    LabelCheckbox.setAttribute("for", `${className}_id`);
    LabelCheckbox.classList.add(`label_${className}`);
    LabelCheckbox.classList.add(`checkbox_label`);
    LabelCheckbox.textContent = Text;

    RemoveBTN.classList.add(`checkbox_remove_btn`);
    IMG_RemoveBTN.innerHTML = svgTrash;
    RemoveBTN.append(IMG_RemoveBTN);

    LabelCheckbox.addEventListener("click", () => handleCheckboxClick(id));
    RemoveBTN.addEventListener("click", () => RemoveBTNClick(id));


    checkbox.append(InputCheckbox);
    checkbox.append(LabelCheckbox);
    checkbox.append(RemoveBTN);

    return checkbox;

  };

  const handleCheckboxClick = (id) => {
    // Найдем соответствующий Gesture по тексту checkbox и выведем его
    const checkbox = Checkboxs.find((item) => item.id === id);
    if (checkbox) {
      let value =!checkbox.checkbox.querySelector('input').checked;
      changeHouse(value,checkbox.id)
    }
  };

  const updateH1 = (data) => {
    const h1 = document.querySelector("h1");
    h1.textContent = data;
  };

  const StartArray = (data) =>{
    const StratJSON = JSON.parse(data);
    StratJSON.forEach(elem => {
      const checkbox = createCheckbox(elem.name, elem.name, elem.id);
      const Checkbox = {
        checkbox: checkbox,
        name: elem.name,
        gesture: elem.gesture,
        pin: elem.pin,
        id: elem.id,
      };
      Checkboxs.push(Checkbox);
      document.querySelector(".Checkboxs").append(checkbox);
      console.log(elem)
    })
  } 

  const updateCheckbox = (gesture) =>{
    const checkbox = Checkboxs.find((item) => item.gesture === gesture);
    if (checkbox) {
      console.log(checkbox);
      let value =!checkbox.checkbox.querySelector('input').checked;
      checkbox.checkbox.querySelector('input').checked = value;
      changeHouse(value,checkbox.id)
    }
  }

  setInterval(() => {
    fetch('/sensor')
      .then((response) => response.text())
      .then((data) => {
        if(data !== ' '){
          if(data !== GestureData){
            GestureData = data;
            updateCheckbox(GestureData);
          }
          updateH1(data);
        } else {
          updateH1('GESTURE');
        }
      });
  }, 300);

  fetch('/get_data')
      .then((response) => response.text())
      .then((data) => {
        StartArray(data);
      });



  let BannerADDCheckbox = true;
  Banner_add_checkbox.style.marginTop = "-180px";


  const ADD_Checkbox = (e) => {
    if (BannerADDCheckbox) {
      e.stopPropagation();
      Banner_add_checkbox.style.marginTop = 'calc(50vh - 115px)';
      Brightness.style.filter = 'brightness(0.5)';
      BannerADDCheckbox = !BannerADDCheckbox;
    }
  };

  const Cancel_Checkbox = () => {
    if (!BannerADDCheckbox) {
      Banner_add_checkbox.style.marginTop = "-180px";
      Brightness.style.filter = 'brightness(1)';
      BannerADDCheckbox = !BannerADDCheckbox;
    }
  };

  ADD_Checkbox_btn.onclick = ADD_Checkbox;
  Brightness.onclick = Cancel_Checkbox;

  const ArrrayCheckbox = (Name, Gesture, Pin) => {
    const id = Date.now();
    const checkbox = createCheckbox(Name, Name, id);
    const Checkbox = {
      checkbox: checkbox,
      name: Name,
      gesture: Gesture,
      pin: Pin,
      id: id,
    };
    Checkboxs.push(Checkbox);
    document.querySelector(".Checkboxs").append(checkbox);
    addNewBoxHouse(Name,Gesture,Pin,'false',id)

  };

  const Input = (placeholder, className) => {
    const input = document.createElement("input");
    input.placeholder = placeholder;
    input.classList.add("Input");
    input.classList.add(className);
    return input;
  };

  const BTN = (text) => {
    const btn = document.createElement("button");
    const span = document.createElement("span");
    span.textContent = text;
    btn.append(span);
    return btn;
  };

  const Banner = () => {
    const banner = document.createElement("div");
    const inputs = document.createElement("div");
    const btns = document.createElement("div");

    banner.classList.add("banner");
    inputs.classList.add("inputs");
    btns.classList.add("btns");

    const Name = Input("Name", "Name");
    const Name_Gesture = Input("Name_Gesture", "Name_Gesture");
    const Num_Pin = Input("Num_Pin", "Num_Pin");

    Num_Pin.type = "number";

    const Cancel = BTN("Cancel");
    Cancel.onclick = Cancel_Checkbox;
    const OK = BTN("OK");

    OK.onclick = () => {
      if (Name.value && Name_Gesture.value && Num_Pin.value) {
        ArrrayCheckbox(Name.value, Name_Gesture.value, Num_Pin.value);
        Cancel_Checkbox();
      }
    };

    inputs.append(Name);
    inputs.append(Name_Gesture);
    inputs.append(Num_Pin);

    btns.append(Cancel);
    btns.append(OK);

    banner.append(inputs);
    banner.append(btns);

    return banner;
  };

  Banner_add_checkbox.append(Banner());


 function addNewBoxHouse(Name,Gesture, Pin, Value,Id) {
    const xhr = new XMLHttpRequest();
    xhr.open('GET', '/addNewBoxHouse?name=' + Name + '&gesture=' + Gesture + '&pin=' + Pin + '&value=' + Value + '&id='+ Id);
    xhr.send();
  }


 function changeHouse(Value,Id) {
    const xhr = new XMLHttpRequest();
    xhr.open('GET', '/changeHouse?value=' + Value + '&id='+ Id);
    xhr.send();
  }
  
   function removeBoxHouse(Id) {
    const xhr = new XMLHttpRequest();
    xhr.open('GET', '/removeBoxHouse?id='+ Id);
    xhr.send();
  }




  const RemoveBTNClick = (id) =>{
    const index = Checkboxs.findIndex((item) => item.id === id);
    if (index !== -1) {
      Checkboxs[index].checkbox.remove()
      Checkboxs.splice(index, 1);
      removeBoxHouse(id);
    } 
  }

    )";
}








void WEBSITE(AsyncWebServerRequest *request) {
  String html = R"(
    <!DOCTYPE html>
    <html lang="en">
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Document</title>
        <style>
  )";

  // Add your CSS styles here
  html += css();

  html += R"(
        </style>
      </head>
          <body>

            <div class="ADD_Checkbox">

            </div>

            <div class="Brightness">
                <header>
                    <h1 class="name_gesture">GESTURE</h1>
                </header>
                <main>
                    <div class="Checkboxs">

                    </div>

                    <div class="range_LED">
                        <input type="range" id="cowbell" name="cowbell" 
                            min="0" max="100" value="90" step="10">
                        <label for="cowbell">LightLED</label>
                    </div>
                    

                    <button class="ADD_Checkbox_btn">
                        <span>
                            ADD
                        </span>
                    </button>

                </main>
                <footer>
                    <div class="github contact">
                      <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                        <g id="icons8-github 1">
                          <g id="Group">
                            <g id="Group_2">
                              <path id="Vector" d="M12 2.25C6.61538 2.25 2.25 6.61538 2.25 12C2.25 16.6039 5.442 20.46 9.73312 21.4823C9.51113 21.4294 9.291 21.3701 
                              9.075 21.3026V19.0687C9.075 19.0687 8.70937 19.1906 8.22187 19.1906C6.858 19.1906 6.29137 17.9738 6.15 17.3625C6.06412 16.9901 5.83987 
                              16.6372 5.59912 16.4216C5.3115 16.1651 5.17688 16.1644 5.175 16.0766C5.17125 15.8925 5.42175 15.9 5.54062 15.9C6.15 15.9 6.612 16.5484 
                              6.8265 16.8836C7.35788 17.7112 7.92825 17.85 8.22187 17.85C8.5875 17.85 8.90325 17.7952 9.12075 17.6903C9.22125 16.9823 9.53625 16.3515 
                              10.05 15.9C7.76362 15.4429 6.15 14.1315 6.15 12C6.15 10.902 6.59063 9.89288 7.32488 9.078C7.24988 8.86538 7.125 8.43525 7.125 7.73438C7.125 
                              7.27125 7.15725 6.70275 7.36875 6.15C7.36875 6.15 8.75925 6.15975 10.0706 7.40175C10.6759 7.2255 11.3235 7.125 12 7.125C12.6765 7.125 13.3241
                              7.2255 13.9294 7.40175C15.2408 6.15975 16.6312 6.15 16.6312 6.15C16.8439 6.70275 16.875 7.27125 16.875 7.73438C16.875 8.49 16.7745 8.93062 16.713
                              9.12075C17.4248 9.92813 17.85 10.9215 17.85 12C17.85 14.1315 16.2364 15.4429 13.95 15.9C14.5605 16.4363 14.925 17.2174 14.925 18.0938V21.3026C14.709 
                              21.3705 14.4892 21.4294 14.2669 21.4823C18.558 20.46 21.75 16.6039 21.75 12C21.75 6.61538 17.3846 2.25 12 2.25ZM12.6799 21.7238C12.4552 21.7395 12.2288 21.75 12 21.75C12.2288 21.75 12.4549 21.7391 12.6799 21.7238ZM12 21.75C11.7712 21.75 11.5448 21.7395 11.3201 21.7238C11.5451 21.7391 11.7712 21.75 12 21.75ZM11.1705 21.7125C10.7141 21.6739 10.2664 21.6041 9.83025 21.5047C10.2668 21.6038 10.7141 21.6739 11.1705 21.7125Z" fill="white"/>
                            </g>
                          </g>
                        </g>
                      </svg>
                      <a href="https://github.com/Evgenij-svg">github</a>
                    </div>
                    <div class="email contact">
                      <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                        <g id="icons8-gmail (1) 1">
                          <g id="Group">
                            <g id="Group_2">
                              <path id="Vector" d="M5.75999 11.2334V11.2272V4.98672L5.70239 4.944H5.69759L4.38719 3.9744C3.58559 3.3792 2.44319 3.408 1.73279 4.1088C1.25759
                                4.5792 0.959991 5.232 0.959991 5.9568V7.68576L5.75999 11.2334ZM18.24 11.2272V11.2334L23.04 7.68576V5.9568C23.04 5.2416 22.752 4.5888 22.2816 4.1184C21.5808 3.41904 20.4744 3.33696 19.6781 3.92496L18.2976 4.944L18.24 4.98672V11.2272ZM6.71999 11.9366L11.7149
                                15.6288C11.8843 15.7541 12.1162 15.7541 12.2856 15.6288L17.28 11.9366V5.69616L12 9.6L6.71999 5.69616V11.9366ZM18.24 12.4267V19.68C18.24
                                19.945 18.455 20.16 18.72 20.16H21.84C22.5029 20.16 23.04 19.6229 23.04 18.96V8.87856L18.24 12.4267ZM5.75999 12.4267L0.959991 8.87856V18.96C0.959991
                                19.6229 1.49711 20.16 2.15999 20.16H5.27999C5.54495 20.16 5.75999 19.945 5.75999 19.68V12.4267Z" fill="white"/>
                            </g>
                          </g>
                        </g>
                      </svg>
                      <a href="mailto:technick.smart.house.md@gmail.com">email</a>
                    </div>
                    <div class="tg contact">
                      <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                        <g id="icons8-&#209;&#130;&#208;&#181;&#208;&#187;&#208;&#181;&#208;&#179;&#209;&#128;&#208;&#176;&#208;&#188; 1">
                          <g id="Group">
                            <g id="Group_2">
                              <path id="Vector" d="M12 0.959999C18.0974 0.959999 23.04 5.90256 23.04 12C23.04 18.0974 18.0974 23.04 12 23.04C5.90255 23.04 0.959991 18.0974 0.959991 12C0.959991
                                5.90256 5.90255 0.959999 12 0.959999ZM15.8083 16.5C16.0114 15.877 16.9627 9.66768 17.0803 8.44416C17.1158 8.0736 16.9987 7.82736 16.7693 7.71744C16.4918 7.584 16.081 7.65072 15.6043 7.82256C14.9506 8.05824 6.59279 11.6069 6.10991 11.8123C5.65199 12.0067 5.21903 12.2189 5.21903 12.5261C5.21903 12.7421 5.34719 12.8635 5.70047 12.9898C6.06815
                                13.1208 6.99407 13.4016 7.54079 13.5523C8.06735 13.6978 8.66687 13.5715 9.00287 13.3627C9.35903 13.1414 13.4693 10.391 13.7645 10.1501C14.0592 9.90912 14.2944 10.2178 14.0534 10.4592C13.8125
                                10.7002 10.991 13.4386 10.619 13.8178C10.1674 14.2781 10.488 14.7552 10.7909 14.9462C11.137 15.1642 13.6258 16.8336 14.0006 17.1014C14.3755 17.3693 14.7557 17.4907 15.1037 17.4907C15.4517 17.4907
                                15.635 17.0323 15.8083 16.5Z" fill="white"/>
                            </g>
                          </g>
                        </g>
                      </svg>
                      <a href="https://t.me/SMART_HOUSE_TECHNICK_MD_bot">telegram</a>
                    </div>
                  </footer>
            </div>
        <script>
  )";

  html += script();

  html += R"(
        </script>
      </body>
    </html>
  )";

  request->send(200, "text/html", html);
}
