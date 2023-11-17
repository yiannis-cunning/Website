
document.getElementById("demo").innerHTML = "Uploaded file: ";

/* upload a file by clicking the btn*/
document.getElementById("upld-file-btn").addEventListener("click", getfile);
function getfile() {
  document.getElementById("uploaded-file").click(); /* prompts user for file */
}



/* Every time the 'input' oject changes -> add file to list*/
document.getElementById("uploaded-file").addEventListener("change", handleFiles, false);
function handleFiles() {
  /* this.files is the list of files */
  document.getElementById("demo").innerHTML = "Uploaded file: " + this.files[0].name; 
  var fr = new FileReader();
  fr.onload=function(){ 
    sendfile(fr.result);
  };
  document.getElementById("main-content").textContent = "Reading + deconstructing binary exe file...";
  fr.readAsArrayBuffer(this.files[0]);
}



function sendfile(arrybuf) {
  /* make the http transmitter object...*/
  let req = new XMLHttpRequest();
  req.open("POST", "upld.exe");
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