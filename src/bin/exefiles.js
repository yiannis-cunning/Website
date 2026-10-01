
document.getElementById("demo").innerHTML = "Uploaded file: ";

/* upload a file by clicking the btn*/
document.getElementById("upld-file-btn").addEventListener("click", getfile);
function getfile() {
  document.getElementById("uploaded-file").click(); /* prompts user for file */
}

document.getElementById("send-file-btn").addEventListener("click", find_send_file);

function find_send_file() {
  if(saved_file != 0){
    document.getElementById("main-content").textContent = "Reading + deconstructing binary exe file...";
    sendfile(saved_file);
  }
  else{
    document.getElementById("main-content").textContent = "No file uploaded yet...";
  }
}

let saved_file = 0;


/* Every time the 'input' oject changes -> add file to list*/
document.getElementById("uploaded-file").addEventListener("change", handleFiles, false);
function handleFiles() {
  /* this.files is the list of files */
  document.getElementById("demo").innerHTML = "Uploaded file: " + this.files[0].name; 
  var fr = new FileReader();
  fr.onload=function(){ 
    //sendfile(fr.result);
    saved_file = fr.result;
  };
  //document.getElementById("main-content").textContent = "Reading + deconstructing binary exe file...";
  fr.readAsArrayBuffer(this.files[0]);
}

 const flags_list = ["c", "s", "w", "e", "t", "t", "q", "v", "i", "r"];

function sendfile(arrybuf) {
  /* make the http transmitter object...*/
  let req = new XMLHttpRequest();

  let to_open = "";
  let flags = document.getElementById("flags-feild").value;

  for (let i = 0; i < flags_list.length; i++) {
    if(flags.indexOf(flags_list[i]) != -1 && to_open.indexOf(flags_list[i]) == -1){
      to_open = to_open + flags_list[i];
    }
  } 
  to_open = "upld.exe" + to_open;
  document.getElementById("demo").innerHTML = to_open;
  
  req.open("POST", to_open);
  /*variable.onload = function() {   comes back to this once recived??
    document.getElementById("send-file-btn").innerHTML = "sent";
  }*/
  req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");

  /* You can read a file with a file reader */
  /*var fr = new FileReader();

  fr.readAsBinaryString()
  */
  req.onload = function(){
    document.getElementById("main-content").textContent = "Done";
    document.getElementById("outframe").src = req.responseText;
  };
  req.send(arrybuf); 

}








/* arg flag controller */



/*
const newelem = document.createElement("BUTTON");
newelem.innerHTML = "BUTTON";
newelem.classList.add("button_css");
newelem.id = "btn" + i.toString();
document.body.appendChild(newelem);

elem = document.getElementById("btn" + i.toString());
elem.addEventListener("click", myFunction);
i += 1;*/

/*
const input = document.createElement('button');
input.classList.add("button_css");
input.innerHTML = "Add a file";
input.type = 'file';
document.body.appendChild(input);
*/