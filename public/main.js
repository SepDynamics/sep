document.addEventListener('DOMContentLoaded', () => {
    fetch('concepts.json')
        .then(response => response.json())
        .then(data => {
            const container = document.getElementById('concepts-container');
            data.concepts.forEach(concept => {
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
        });
});