
let urls_clients = [];
let urls_PAJ = [];

let curentOBJ = {};

function AddCommand(){
    const InputCommands = document.querySelector(".InputCommands");
    curentOBJ['commands'].push(InputCommands.value);
    send_add_command(curentOBJ['nameUrl'],InputCommands.value)

    const Comands = document.querySelector(".Comands");
    const Command = document.createElement("span");
    Command.innerText = InputCommands.value
    Comands.append(Command);
    InputCommands.value = "";

    

}

function send_add_command(name_url,command){

    fetch('/add_command', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'name_url=' + encodeURIComponent(name_url.split('/')[2]) + '&command=' + encodeURIComponent(command),
    })
}


function remove_url_block_from_serv(name_url){
    fetch('/remove_url', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'name_url=' + encodeURIComponent(name_url),
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

function create_url_client_block(url_block_client){
    const Name_url =  url_block_client['nameUrl'].split('/')[2]


    const deleteUrlClient = function(){
        urls_clients = urls_clients.filter(
            (elem_remove) => elem_remove['nameUrl'].split('/')[2] != url_block_client['nameUrl'].split('/')[2]        
        );
        Client_url.remove();
        remove_url_block_from_serv(url_block_client['nameUrl'].split('/')[2])
    }

    const addCommadUrlClient = function(event){
        curentOBJ = urls_clients.filter(
            (elem) => elem['nameUrl'].split('/')[2] == url_block_client['nameUrl'].split('/')[2]        
        )[0];

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

function create_url_PAJ_block(IP_PAJ){
    const deleteUrlPAJ = function(){
        urls_PAJ = urls_PAJ.filter(
            (elem_remove) => elem_remove != IP_PAJ        
        );
        PAJ_url.remove();
        // remove_url_block_from_serv(url_block_client['nameUrl'].split('/')[2])
    }

    const delete_button = create_delete_button(deleteUrlPAJ)
    const PAJ_url = create_url_block(IP_PAJ, delete_button)
    
    return PAJ_url
}

function RenderUrls(){
    Urls=document.querySelector(".Urls");
    urls_clients.forEach((elem)=>{
        block_url = create_url_client_block(elem)
        Urls.appendChild(block_url);
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
        urls_clients.splice(0, urls_clients.length);
        const temp = JSON.parse(data)
        urls_clients = [...temp]
        RenderUrls()
        console.log(urls_clients)
    })
}
getData()

function addUrl() {
    let inputData = document.querySelector('#inputData').value;

    // Используем Fetch API
    fetch('/add_urls', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'input_data=' + encodeURIComponent(inputData),
    })
    .then(response => response.text())
    .then(data => {
        if(data == "True"){
            url = 'http://' + inputData + '/Gesture_data'
            url_client = {
                'nameUrl':url,
                'commands':[]
            };
            urls_clients.push(url_client);
            console.log(inputData);

            const url_client_block = create_url_client_block(url_client);
            const Urls=document.querySelector(".Urls");
            Urls.appendChild(url_client_block);

        }
    })
}


const socket = io.connect('http://192.168.0.14:5000'); // Adjust the URL accordingly

socket.on('esp_ip_update', function (data) {
    console.log('Received update:', data);
    const url_PAJ_block = create_url_PAJ_block(data);
    urls_clients.push(data);
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


