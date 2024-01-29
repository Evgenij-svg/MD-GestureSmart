let curentOBJ = {};

let urls = {
    "clients":{
        "socket":[],
        "tcp":[],
        "post":[],
        "get":[],
    },
    "paj":[]
}

let urls_clients = urls['clients'];


function AddCommand(){
    const InputCommands = document.querySelector(".InputCommands");

    const valueInputCommands = InputCommands.value;
    InputCommands.value = '';

    curentOBJ['commands'].push(valueInputCommands);
    send_add_command(curentOBJ['id'],valueInputCommands,curentOBJ['_request'])
    const Comands = document.querySelector(".Comands");
    const Command = document.createElement("div");
    const Command_name =  document.createElement("span");
    Command_name.innerText = valueInputCommands;


    const Command_delete_btn_func = function(){
        const index = curentOBJ['commands'].indexOf(valueInputCommands);
        curentOBJ['commands'].splice(index, 1);
        Command.remove();

    }
    const Command_delete_btn =  create_delete_button(Command_delete_btn_func);


    Command.append(Command_name);
    Command.append(Command_delete_btn);

    Comands.append(Command);
    // InputCommands.value = "";

    

}

function send_add_command(id_url,command,_request){

    fetch('/add_command', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'id_url=' + encodeURIComponent(id_url) + '&command=' + encodeURIComponent(command) + '&_request=' + encodeURIComponent(_request),
    })
}

function remove_url_block_from_serv(id_url,_request){
    fetch('/remove_url', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'id_url=' + encodeURIComponent(id_url) + '&_request=' + encodeURIComponent(_request),
    })
}

function remove_url_paj_block_from_serv(id_paj){
    fetch('/remove_url_paj', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'id_paj=' + encodeURIComponent(id_paj),
    })
}

function create_url_block(URL_text,...Buttons){
    const block_url = document.createElement("div");
    
    const url = document.createElement("span");
    url.innerText =  URL_text;
    url.classList.add('url_text');
    block_url.appendChild(url);
    block_url.classList.add('url_block');

    const block_buttons = document.createElement("div")

    for (let i = 0; i < Buttons.length; i++) {
        block_buttons.appendChild(Buttons[i]);
    }


    block_url.append(block_buttons);

    return block_url;

}

function create_delete_button(DeleteBtnFunc){
    const deletUrl = document.createElement("button");
    const trash_img = document.createElement("img");
    trash_img.src = "./static/img/trash.png"
    deletUrl.classList.add("remove_url");
    deletUrl.appendChild(trash_img);
    deletUrl.onclick = DeleteBtnFunc;
    return deletUrl
}

function create_command_button(addCommandFunc){
    const command_btn = document.createElement("button");
    const command_img = document.createElement("img");
    command_img.src = "./static/img/command.png"
    command_btn.classList.add("command_btn");
    command_btn.appendChild(command_img);
    command_btn.onclick = addCommandFunc;
    return command_btn;
}

function create_url_client_block(url_block_client,_request){
    const Name_url =  url_block_client['nameUrl'];

    const deleteUrlClient = function(){
        urls_clients[_request] = urls_clients[_request].filter(
            (elem_remove) => elem_remove['id'] != url_block_client['id']    
        );
        Client_url.remove();
        remove_url_block_from_serv(url_block_client['id'],_request)
    }

    const addCommadUrlClient = function(event){
        curentOBJ = urls_clients[_request].filter(
            (elem) => elem['id'] == url_block_client['id']    
        )[0];
        
        curentOBJ['_request'] = _request;
        document.querySelector(".AddComandsBlock").classList.add("AddComandsBlockActive");
        document.querySelector("main").classList.add("mainActive");

        const Comands = document.querySelector(".Comands");

        curentOBJ['commands'].forEach(elem => {
            const Command = document.createElement("span");
            Command.innerText = elem
            Comands.append(Command);
        })

        document.querySelector(".mainActive").onclick = function(){
            document.querySelector("main").classList.remove("mainActive");
            document.querySelector(".AddComandsBlock").classList.remove("AddComandsBlockActive");

            while (Comands.firstChild) {
                Comands.removeChild(Comands.firstChild);
            }

        }
        event.stopPropagation()
    }

    const command_btn = create_command_button(addCommadUrlClient);
    const delete_button = create_delete_button(deleteUrlClient)
    const Client_url = create_url_block(Name_url,command_btn, delete_button)
    
    return Client_url
}

function create_url_PAJ_block(PAJ_block){
    const deleteUrlPAJ = function(){
        urls['paj'] = urls['paj'].filter(
            (elem_remove) => elem_remove['id'] != PAJ_block['id']       
        );
        PAJ_url.remove();
        remove_url_paj_block_from_serv(PAJ_block['id'])
    }

    const delete_button = create_delete_button(deleteUrlPAJ)
    const PAJ_url = create_url_block(PAJ_block['ipPaj'], delete_button)
    
    return PAJ_url
}

function RenderUrls(){
    const Get_dom=document.querySelector(".Get");
    urls_clients['get'].forEach((elem)=>{
        block_url = create_url_client_block(elem,'get')
        Get_dom.appendChild(block_url);
    })

    const Post_dom=document.querySelector(".Post");
    urls_clients['post'].forEach((elem)=>{
        block_url = create_url_client_block(elem,'post')
        Post_dom.appendChild(block_url);
    })

    const Socket_dom=document.querySelector(".Socket");
    urls_clients['socket'].forEach((elem)=>{
        block_url = create_url_client_block(elem,'socket')
        Socket_dom.appendChild(block_url);
    })
}

function getData(){
    fetch('/get_urls', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
    })
    .then(response => response.text())
    .then(data => {    
        Object.keys(urls).forEach(item => delete urls[item]);
        const temp = JSON.parse(data)
        urls = {...temp}
        urls_clients = urls['clients']
        RenderUrls()
        console.log(urls)
    })
}
getData()

function addUrl() {
    let inputData = document.querySelector('#inputData').value;
    const id_url = Date.now();
    let _request = ''
    let radioButtons = document.getElementsByName('request_');
    for (let i = 0; i < radioButtons.length; i++) {
        if (radioButtons[i].checked) {
            _request = radioButtons[i].value;
            break;
        }
    }
    // Используем Fetch API
    fetch('/add_urls', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'url=' + encodeURIComponent(inputData) + '&id_url=' + encodeURIComponent(id_url) +'&_request=' +encodeURIComponent(_request),
    })
    .then(response => response.text())
    .then(data => {
        if(data == "True"){
            // url = 'http://' + inputData + '/Gesture_data'
            url_client = {
                'nameUrl':inputData,
                'commands':[],
                'id':id_url
            };


            urls_clients[_request].push(url_client);

            console.log(urls_clients);

            const url_client_block = create_url_client_block(url_client,_request);

            let UrlBlock=document.querySelector(".Get");
            if(_request === 'post'){
                UrlBlock=document.querySelector(".Post");
            }else{
                UrlBlock=document.querySelector(".Socket");
            }
            UrlBlock.appendChild(url_client_block);


        }
    })
}

const socket = io.connect('http://192.168.0.14:5000'); // Adjust the URL accordingly

socket.on('esp_ip_update', function (data) {
    const _data = JSON.parse(data)
    console.log('Received update:',  _data);
    const url_PAJ_block = create_url_PAJ_block(_data);
    urls['paj'].push(_data);

    const MD_GESTURE_PAJ = document.querySelector(".MD_GESTURE_PAJ");
    MD_GESTURE_PAJ.appendChild(url_PAJ_block);
    // Process data from the server
});

async function search() {
    try {
        const response = await fetch('/Search_MD_GESTURE_PAJ', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: '',
        });

        if (!response.ok) {
            console.error('Failed to fetch data from server');
            return;
        }else {
            const data = await response.json();
            console.log("END")
            console.log(data);
        }
    } catch (error) {
        console.error('An error occurred:', error);
    }
}


