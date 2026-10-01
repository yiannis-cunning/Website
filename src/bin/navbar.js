// From google AI
async function loadContent(id, filename) {
    try {
      // 1. Fetch the external HTML file
      const response = await fetch(filename);
      
      // 2. Check if the request was successful
      if (!response.ok) {
        throw new Error(`Failed to load file: ${response.statusText}`);
      }
  
      // 3. Convert the response to text
      const html = await response.text();
  
      // 4. Inject the HTML into the target div
      document.getElementById(id).innerHTML = html;
    } catch (error) {
      console.error('Error loading the HTML file:', error);
    }
  }
  
  // Call the function when the page loads or on a specific event
loadContent('nav-bar', 'http://cunning.twilightparadox.com/navbar.html');
