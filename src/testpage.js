

document.getElementById("regex").addEventListener("click", function(){setcontent("regex.html");});


document.getElementById("gohome").addEventListener("click", function(){document.getElementById('maincontent').innerHTML = "Homepage";});

function setcontent(newcontent) {
    var txtFile = new XMLHttpRequest();
    txtFile.open("GET", newcontent, true);
    txtFile.onreadystatechange = function() {
      if (txtFile.readyState === 4) {  // Makes sure the document is ready to parse.
        if (txtFile.status === 200) {  // Makes sure it's found the file.
          allText = txtFile.responseText; 
          //lines = txtFile.responseText.split("\n"); // Will separate each line into an array
          var customTextElement = document.getElementById('maincontent');
         customTextElement.innerHTML = txtFile.responseText;
        }
      }
    }
    txtFile.send(null);
}
