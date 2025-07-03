async function generateEmbeddings(text) {
  const response = await fetch('http://localhost:8080/pattern/analyze', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      text,
      options: {
        return_embeddings: true
      }
    })
  });

  if (!response.ok) {
    throw new Error(`HTTP error! status: ${response.status}`);
  }

  const data = await response.json();
  return data;
}

// Example usage
async function main() {
  try {
    const result = await generateEmbeddings("Your input text here");
    console.log('Generated embeddings:', result);
  } catch (error) {
    console.error('Error:', error);
  }
}

main();