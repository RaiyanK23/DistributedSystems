// Configuration for the fetch request
const stock = "ALPHABET";
const requestOptions = {
  method: 'POST',
  headers: {
    'Content-Type': 'text/plain', // Specify the content type as text/plain
  },
  body: stock, // Set the request body to the string you want to send
};

// Function to send an HTTP POST request to the proxy server with retry logic
const sendPostRequestWithRetry = async (proxyUrl) => {
  try {
    // Send the POST request to the proxy server
    const response = await fetch(proxyUrl, requestOptions);

    if (!response.ok) {
      throw new Error('Network response was not ok');
    }

    // Return the response text
    return await response.text();
  } catch (error) {
    console.error('Error:', error);
    throw error; // Re-throw the error to be caught by the caller
  }
};

// Function to determine the next proxy URL to try based on the current URL
const getNextProxyUrl = (currentProxyUrl) => {
  // Define backup proxy URLs
  const backupProxyUrls = ['http://localhost:8081', 'http://localhost:8082'];
  // Find the index of the current URL in the backup URLs array
  const currentIndex = backupProxyUrls.indexOf(currentProxyUrl);
  // If the current URL is not found in the backup URLs array or is the last URL,
  // return the first URL in the array as the next URL to try
  if (currentIndex === -1 || currentIndex === backupProxyUrls.length - 1) {
    return backupProxyUrls[0];
  }
  // Otherwise, return the next URL in the array as the next URL to try
  return backupProxyUrls[currentIndex + 1];
};

// Function to repeatedly send POST requests and wait for a response with retry logic
const waitForResponseWithRetry = async () => {
  let proxyUrl = 'http://localhost:8080'; // Initial proxy URL
  let responseText = null;
  let retries = 0;
  const maxRetries = 2; // Maximum number of retries

  while (!responseText && retries <= maxRetries) {
    try {
      // Send the POST request to the current proxy URL
      responseText = await sendPostRequestWithRetry(proxyUrl);
    } catch (error) {
      // Handle errors
      console.error('Error:', error);
      // If the error indicates a fetch failure, try the next proxy URL
      proxyUrl = getNextProxyUrl(proxyUrl);
      console.log(`Retrying with proxy URL: ${proxyUrl}`);
      retries++;
    }
    // Check for the response every second
    await new Promise(resolve => setTimeout(resolve, 1000));
  }

  // Display the response from the proxy server if available
  if (responseText) {
    // Parse the JSON data received from the proxy
    try {
      const jsonData = JSON.parse(responseText);
      // Log the parsed JSON data to the console
      console.log('JSON Data received from proxy:', jsonData);
    } catch (error) {
      console.error('Error parsing JSON data:', error);
    }
  } else {
    console.error('Maximum retries reached. Unable to connect to proxy server.');
  }
};

// Start waiting for the response with retry logic
waitForResponseWithRetry();
