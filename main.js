document.addEventListener('DOMContentLoaded', () => {
    const container = document.getElementById('concepts-container');
    const searchInput = document.getElementById('search-input');
    let allConcepts = [];

    // Load the generated concepts list from the repository root.
    // This path was previously "public/concepts.json" but the file
    // was moved to the project root to simplify deployment.
    fetch('concepts.json')
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            allConcepts = data.concepts;
            renderConcepts(allConcepts);
        })
        .catch(error => {
            console.error("Could not load concepts:", error);
            container.innerHTML = '<p>Error loading concepts. Please try again later.</p>';
        });

    function renderConcepts(concepts) {
        container.innerHTML = '';
        if (concepts.length === 0) {
            container.innerHTML = '<p>No concepts found.</p>';
            return;
        }
        concepts.forEach(concept => {
            const div = document.createElement('div');
            div.className = 'concept';

            const h2 = document.createElement('h2');
            h2.textContent = concept.name;
            div.appendChild(h2);

            const p = document.createElement('p');
            p.textContent = concept.description;
            div.appendChild(p);

            container.appendChild(div);
        });
    }

    searchInput.addEventListener('input', (e) => {
        const searchTerm = e.target.value.toLowerCase();
        const filteredConcepts = allConcepts.filter(concept => {
            return concept.name.toLowerCase().includes(searchTerm) ||
                   concept.description.toLowerCase().includes(searchTerm);
        });
        renderConcepts(filteredConcepts);
    });
});