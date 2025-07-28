// SEP Dynamics Website JavaScript

document.addEventListener('DOMContentLoaded', function() {
    // Initialize all components
    initNavigation();
    initHeroAnimation();
    initPerformanceChart();
    initDemoSection();
    initProjectionChart();
    initPatentDemos();
    initScrollAnimations();
});

// Navigation functionality
function initNavigation() {
    const hamburger = document.querySelector('.hamburger');
    const navMenu = document.querySelector('.nav-menu');
    
    if (hamburger) {
        hamburger.addEventListener('click', function() {
            hamburger.classList.toggle('active');
            navMenu.classList.toggle('active');
        });
    }
    
    // Smooth scroll for navigation links
    const navLinks = document.querySelectorAll('.nav-menu a[href^="#"]');
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href');
            const targetSection = document.querySelector(targetId);
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
                // Close mobile menu if open
                navMenu.classList.remove('active');
                hamburger.classList.remove('active');
            }
        });
    });
}

// Hero section animation
function initHeroAnimation() {
    const heroCanvas = document.getElementById('hero-canvas');
    if (!heroCanvas) return;
    
    const ctx = heroCanvas.getContext('2d');
    const width = heroCanvas.width;
    const height = heroCanvas.height;
    
    // Set up canvas for high DPI displays
    const dpr = window.devicePixelRatio || 1;
    heroCanvas.width = width * dpr;
    heroCanvas.height = height * dpr;
    ctx.scale(dpr, dpr);
    
    // Animation state
    let time = 0;
    const particles = [];
    const patternData = [];
    
    // Initialize particles and pattern data
    for (let i = 0; i < 100; i++) {
        particles.push({
            x: Math.random() * width,
            y: Math.random() * height,
            vx: (Math.random() - 0.5) * 2,
            vy: (Math.random() - 0.5) * 2,
            size: Math.random() * 3 + 1,
            coherence: Math.random(),
            stability: Math.random(),
            entropy: Math.random()
        });
    }
    
    // Generate sample pattern data
    for (let i = 0; i < width; i += 2) {
        patternData.push({
            x: i,
            coherence: 0.5 + 0.3 * Math.sin(i * 0.02) + 0.1 * Math.random(),
            stability: 0.6 + 0.2 * Math.cos(i * 0.015) + 0.1 * Math.random(),
            entropy: 0.3 + 0.2 * Math.sin(i * 0.03) + 0.1 * Math.random()
        });
    }
    
    function animate() {
        time += 0.016; // ~60fps
        
        // Clear canvas
        ctx.fillStyle = 'rgba(10, 10, 10, 0.1)';
        ctx.fillRect(0, 0, width, height);
        
        // Draw grid pattern
        ctx.strokeStyle = 'rgba(102, 126, 234, 0.1)';
        ctx.lineWidth = 1;
        for (let i = 0; i < width; i += 40) {
            ctx.beginPath();
            ctx.moveTo(i, 0);
            ctx.lineTo(i, height);
            ctx.stroke();
        }
        for (let i = 0; i < height; i += 40) {
            ctx.beginPath();
            ctx.moveTo(0, i);
            ctx.lineTo(width, i);
            ctx.stroke();
        }
        
        // Draw coherence pattern
        ctx.strokeStyle = '#667eea';
        ctx.lineWidth = 2;
        ctx.beginPath();
        patternData.forEach((point, index) => {
            const y = height - (point.coherence * height * 0.3) - height * 0.1;
            if (index === 0) {
                ctx.moveTo(point.x, y);
            } else {
                ctx.lineTo(point.x, y);
            }
        });
        ctx.stroke();
        
        // Draw stability pattern
        ctx.strokeStyle = '#764ba2';
        ctx.lineWidth = 2;
        ctx.beginPath();
        patternData.forEach((point, index) => {
            const y = height - (point.stability * height * 0.3) - height * 0.4;
            if (index === 0) {
                ctx.moveTo(point.x, y);
            } else {
                ctx.lineTo(point.x, y);
            }
        });
        ctx.stroke();
        
        // Draw entropy pattern
        ctx.strokeStyle = '#ff6b6b';
        ctx.lineWidth = 1;
        ctx.beginPath();
        patternData.forEach((point, index) => {
            const y = height - (point.entropy * height * 0.3) - height * 0.7;
            if (index === 0) {
                ctx.moveTo(point.x, y);
            } else {
                ctx.lineTo(point.x, y);
            }
        });
        ctx.stroke();
        
        // Update and draw particles
        particles.forEach(particle => {
            // Update position
            particle.x += particle.vx;
            particle.y += particle.vy;
            
            // Bounce off edges
            if (particle.x <= 0 || particle.x >= width) particle.vx *= -1;
            if (particle.y <= 0 || particle.y >= height) particle.vy *= -1;
            
            // Update quantum properties
            particle.coherence = 0.5 + 0.3 * Math.sin(time + particle.x * 0.01);
            particle.stability = 0.5 + 0.3 * Math.cos(time + particle.y * 0.01);
            particle.entropy = 0.3 + 0.2 * Math.sin(time * 2 + particle.x * 0.005);
            
            // Draw particle
            const alpha = particle.coherence * 0.8;
            ctx.fillStyle = `rgba(102, 126, 234, ${alpha})`;
            ctx.beginPath();
            ctx.arc(particle.x, particle.y, particle.size, 0, Math.PI * 2);
            ctx.fill();
            
            // Draw connections to nearby particles
            particles.forEach(otherParticle => {
                const dx = particle.x - otherParticle.x;
                const dy = particle.y - otherParticle.y;
                const distance = Math.sqrt(dx * dx + dy * dy);
                
                if (distance < 100 && distance > 0) {
                    const alpha = (1 - distance / 100) * particle.coherence * 0.3;
                    ctx.strokeStyle = `rgba(102, 126, 234, ${alpha})`;
                    ctx.lineWidth = 1;
                    ctx.beginPath();
                    ctx.moveTo(particle.x, particle.y);
                    ctx.lineTo(otherParticle.x, otherParticle.y);
                    ctx.stroke();
                }
            });
        });
        
        // Update metric displays
        updateHeroMetrics(particles);
        
        requestAnimationFrame(animate);
    }
    
    animate();
}

function updateHeroMetrics(particles) {
    if (!particles || particles.length === 0) return;
    
    // Calculate average metrics
    const avgCoherence = particles.reduce((sum, p) => sum + p.coherence, 0) / particles.length;
    const avgStability = particles.reduce((sum, p) => sum + p.stability, 0) / particles.length;
    const avgEntropy = particles.reduce((sum, p) => sum + p.entropy, 0) / particles.length;
    
    // Update displays
    const coherenceEl = document.getElementById('coherence-value');
    const stabilityEl = document.getElementById('stability-value');
    const entropyEl = document.getElementById('entropy-value');
    
    if (coherenceEl) coherenceEl.textContent = avgCoherence.toFixed(2);
    if (stabilityEl) stabilityEl.textContent = avgStability.toFixed(2);
    if (entropyEl) entropyEl.textContent = avgEntropy.toFixed(2);
}

// Performance chart
function initPerformanceChart() {
    const canvas = document.getElementById('performance-canvas');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const width = canvas.width;
    const height = canvas.height;
    
    // Set up canvas for high DPI displays
    const dpr = window.devicePixelRatio || 1;
    canvas.width = width * dpr;
    canvas.height = height * dpr;
    ctx.scale(dpr, dpr);
    
    // Sample performance data
    const data = [
        { time: '00:00', accuracy: 58, alpha: -0.002, volume: 1200 },
        { time: '04:00', accuracy: 62, alpha: 0.001, volume: 1850 },
        { time: '08:00', accuracy: 59, alpha: -0.001, volume: 2100 },
        { time: '12:00', accuracy: 65, alpha: 0.003, volume: 2800 },
        { time: '16:00', accuracy: 67, alpha: 0.005, volume: 3200 },
        { time: '20:00', accuracy: 63, alpha: 0.002, volume: 2400 },
        { time: '24:00', accuracy: 65, alpha: 0.004, volume: 1800 },
        { time: '28:00', accuracy: 68, alpha: 0.006, volume: 2200 },
        { time: '32:00', accuracy: 66, alpha: 0.004, volume: 2600 },
        { time: '36:00', accuracy: 70, alpha: 0.008, volume: 3100 },
        { time: '40:00', accuracy: 65, alpha: 0.003, volume: 2900 },
        { time: '44:00', accuracy: 67, alpha: 0.005, volume: 2500 },
        { time: '48:00', accuracy: 65, alpha: 0.0084, volume: 2100 }
    ];
    
    function drawChart() {
        // Clear canvas
        ctx.fillStyle = '#000';
        ctx.fillRect(0, 0, width, height);
        
        // Draw grid
        ctx.strokeStyle = 'rgba(255, 255, 255, 0.1)';
        ctx.lineWidth = 1;
        
        // Vertical grid lines
        for (let i = 0; i <= 12; i++) {
            const x = (i / 12) * width;
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, height);
            ctx.stroke();
        }
        
        // Horizontal grid lines
        for (let i = 0; i <= 8; i++) {
            const y = (i / 8) * height;
            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(width, y);
            ctx.stroke();
        }
        
        // Draw accuracy line
        ctx.strokeStyle = '#667eea';
        ctx.lineWidth = 3;
        ctx.beginPath();
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width;
            const y = height - ((point.accuracy - 50) / 20) * height * 0.8 - height * 0.1;
            if (index === 0) {
                ctx.moveTo(x, y);
            } else {
                ctx.lineTo(x, y);
            }
        });
        ctx.stroke();
        
        // Draw alpha bars
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width;
            const barHeight = Math.abs(point.alpha) * 10000; // Scale alpha values
            const y = height * 0.9;
            
            ctx.fillStyle = point.alpha >= 0 ? 'rgba(102, 255, 102, 0.6)' : 'rgba(255, 102, 102, 0.6)';
            ctx.fillRect(x - 10, y - barHeight, 20, barHeight);
        });
        
        // Draw labels
        ctx.fillStyle = '#ccc';
        ctx.font = '12px Inter';
        ctx.textAlign = 'center';
        
        // Time labels
        data.forEach((point, index) => {
            if (index % 2 === 0) {
                const x = (index / (data.length - 1)) * width;
                ctx.fillText(point.time, x, height - 5);
            }
        });
        
        // Accuracy labels
        ctx.textAlign = 'right';
        for (let i = 50; i <= 70; i += 5) {
            const y = height - ((i - 50) / 20) * height * 0.8 - height * 0.1;
            ctx.fillText(i + '%', width - 10, y + 4);
        }
        
        // Legend
        ctx.textAlign = 'left';
        ctx.fillStyle = '#667eea';
        ctx.fillText('● Accuracy', 20, 30);
        ctx.fillStyle = 'rgba(102, 255, 102, 0.8)';
        ctx.fillText('■ Alpha Generation', 20, 50);
    }
    
    drawChart();
}

// Demo section functionality
function initDemoSection() {
    const startBtn = document.getElementById('start-demo');
    const demoCanvas = document.getElementById('demo-canvas');
    
    if (!startBtn || !demoCanvas) return;
    
    let demoRunning = false;
    let demoInterval;
    
    startBtn.addEventListener('click', function() {
        if (demoRunning) {
            stopDemo();
        } else {
            startDemo();
        }
    });
    
    function startDemo() {
        demoRunning = true;
        startBtn.textContent = 'Stop Analysis';
        startBtn.classList.add('btn-danger');
        
        const ctx = demoCanvas.getContext('2d');
        const width = demoCanvas.width;
        const height = demoCanvas.height;
        
        // Set up canvas for high DPI displays
        const dpr = window.devicePixelRatio || 1;
        demoCanvas.width = width * dpr;
        demoCanvas.height = height * dpr;
        ctx.scale(dpr, dpr);
        
        let time = 0;
        const dataPoints = [];
        
        demoInterval = setInterval(() => {
            time += 1;
            
            // Generate simulated market data
            const basePrice = 1.1000;
            const price = basePrice + 0.01 * Math.sin(time * 0.1) + 0.005 * Math.random();
            const volume = 1000 + 500 * Math.random();
            
            // Calculate quantum metrics (simulated)
            const coherence = 0.5 + 0.3 * Math.sin(time * 0.05) + 0.1 * Math.random();
            const stability = 0.6 + 0.2 * Math.cos(time * 0.03) + 0.1 * Math.random();
            const entropy = 0.3 + 0.2 * Math.sin(time * 0.08) + 0.1 * Math.random();
            
            dataPoints.push({ time, price, volume, coherence, stability, entropy });
            
            // Keep only last 100 points
            if (dataPoints.length > 100) {
                dataPoints.shift();
            }
            
            // Update chart
            drawDemoChart(ctx, width, height, dataPoints);
            
            // Update metrics
            updateDemoMetrics(coherence, stability, entropy);
            
            // Generate trading signal
            updateTradingSignal(coherence, stability);
            
        }, 500); // Update every 500ms
    }
    
    function stopDemo() {
        demoRunning = false;
        startBtn.textContent = 'Start Live Analysis';
        startBtn.classList.remove('btn-danger');
        
        if (demoInterval) {
            clearInterval(demoInterval);
        }
    }
    
    function drawDemoChart(ctx, width, height, data) {
        // Clear canvas
        ctx.fillStyle = '#000';
        ctx.fillRect(0, 0, width, height);
        
        if (data.length < 2) return;
        
        // Draw grid
        ctx.strokeStyle = 'rgba(255, 255, 255, 0.1)';
        ctx.lineWidth = 1;
        for (let i = 0; i <= 10; i++) {
            const x = (i / 10) * width;
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, height);
            ctx.stroke();
            
            const y = (i / 10) * height;
            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(width, y);
            ctx.stroke();
        }
        
        // Find price range
        const prices = data.map(d => d.price);
        const minPrice = Math.min(...prices);
        const maxPrice = Math.max(...prices);
        const priceRange = maxPrice - minPrice || 0.01;
        
        // Draw price line
        ctx.strokeStyle = '#ffffff';
        ctx.lineWidth = 2;
        ctx.beginPath();
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width;
            const y = height - ((point.price - minPrice) / priceRange) * height * 0.8 - height * 0.1;
            if (index === 0) {
                ctx.moveTo(x, y);
            } else {
                ctx.lineTo(x, y);
            }
        });
        ctx.stroke();
        
        // Draw coherence overlay
        ctx.strokeStyle = '#667eea';
        ctx.lineWidth = 1;
        ctx.beginPath();
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width;
            const y = height - point.coherence * height * 0.3;
            if (index === 0) {
                ctx.moveTo(x, y);
            } else {
                ctx.lineTo(x, y);
            }
        });
        ctx.stroke();
        
        // Draw current price
        const currentPrice = data[data.length - 1].price;
        ctx.fillStyle = '#667eea';
        ctx.font = '14px Inter';
        ctx.textAlign = 'right';
        ctx.fillText(currentPrice.toFixed(5), width - 10, 20);
    }
    
    function updateDemoMetrics(coherence, stability, entropy) {
        // Update metric bars
        const coherenceBar = document.getElementById('coherence-bar');
        const stabilityBar = document.getElementById('stability-bar');
        const entropyBar = document.getElementById('entropy-bar');
        
        const coherenceNum = document.getElementById('coherence-num');
        const stabilityNum = document.getElementById('stability-num');
        const entropyNum = document.getElementById('entropy-num');
        
        if (coherenceBar) coherenceBar.style.width = (coherence * 100) + '%';
        if (stabilityBar) stabilityBar.style.width = (stability * 100) + '%';
        if (entropyBar) entropyBar.style.width = (entropy * 100) + '%';
        
        if (coherenceNum) coherenceNum.textContent = coherence.toFixed(2);
        if (stabilityNum) stabilityNum.textContent = stability.toFixed(2);
        if (entropyNum) entropyNum.textContent = entropy.toFixed(2);
    }
    
    function updateTradingSignal(coherence, stability) {
        const signalIndicator = document.getElementById('signal-indicator');
        const confidenceEl = document.getElementById('confidence');
        
        if (!signalIndicator || !confidenceEl) return;
        
        const confidence = (coherence * 0.6 + stability * 0.4) * 100;
        confidenceEl.textContent = Math.round(confidence);
        
        const signalIcon = signalIndicator.querySelector('.signal-icon');
        const signalText = signalIndicator.querySelector('.signal-text');
        
        if (coherence > 0.85 && stability > 0.60) {
            signalIcon.textContent = '📈';
            signalText.textContent = 'BUY SIGNAL';
            signalText.style.color = '#4ade80';
        } else if (coherence < 0.3 || stability < 0.4) {
            signalIcon.textContent = '📉';
            signalText.textContent = 'SELL SIGNAL';
            signalText.style.color = '#f87171';
        } else {
            signalIcon.textContent = '📊';
            signalText.textContent = 'ANALYZING...';
            signalText.style.color = '#667eea';
        }
    }
}

// Projection chart
function initProjectionChart() {
    const canvas = document.getElementById('projection-canvas');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    const width = canvas.width;
    const height = canvas.height;
    
    // Set up canvas for high DPI displays
    const dpr = window.devicePixelRatio || 1;
    canvas.width = width * dpr;
    canvas.height = height * dpr;
    ctx.scale(dpr, dpr);
    
    const data = [
        { year: '2025', revenue: 0.2, licensing: 0.1, performance: 0.05, saas: 0.05 },
        { year: '2026', revenue: 1.5, licensing: 0.8, performance: 0.4, saas: 0.3 },
        { year: '2027', revenue: 5.2, licensing: 2.5, performance: 1.8, saas: 0.9 },
        { year: '2028', revenue: 12.8, licensing: 6.0, performance: 4.2, saas: 2.6 },
        { year: '2029', revenue: 28.5, licensing: 12.0, performance: 10.5, saas: 6.0 }
    ];
    
    function drawProjectionChart() {
        // Clear canvas
        ctx.fillStyle = '#111';
        ctx.fillRect(0, 0, width, height);
        
        // Draw grid
        ctx.strokeStyle = 'rgba(255, 255, 255, 0.1)';
        ctx.lineWidth = 1;
        
        for (let i = 0; i <= 5; i++) {
            const x = (i / 5) * width * 0.8 + width * 0.1;
            ctx.beginPath();
            ctx.moveTo(x, height * 0.1);
            ctx.lineTo(x, height * 0.9);
            ctx.stroke();
        }
        
        for (let i = 0; i <= 6; i++) {
            const y = (i / 6) * height * 0.8 + height * 0.1;
            ctx.beginPath();
            ctx.moveTo(width * 0.1, y);
            ctx.lineTo(width * 0.9, y);
            ctx.stroke();
        }
        
        // Draw bars
        const barWidth = (width * 0.8) / data.length / 1.5;
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width * 0.8 + width * 0.1 - barWidth / 2;
            const maxHeight = height * 0.8;
            const maxValue = 30; // Max revenue in millions
            
            // Licensing (bottom)
            const licensingHeight = (point.licensing / maxValue) * maxHeight;
            ctx.fillStyle = '#667eea';
            ctx.fillRect(x, height * 0.9 - licensingHeight, barWidth, licensingHeight);
            
            // Performance (middle)
            const performanceHeight = (point.performance / maxValue) * maxHeight;
            ctx.fillStyle = '#764ba2';
            ctx.fillRect(x, height * 0.9 - licensingHeight - performanceHeight, barWidth, performanceHeight);
            
            // SaaS (top)
            const saasHeight = (point.saas / maxValue) * maxHeight;
            ctx.fillStyle = '#4ade80';
            ctx.fillRect(x, height * 0.9 - licensingHeight - performanceHeight - saasHeight, barWidth, saasHeight);
        });
        
        // Draw labels
        ctx.fillStyle = '#ccc';
        ctx.font = '12px Inter';
        ctx.textAlign = 'center';
        
        data.forEach((point, index) => {
            const x = (index / (data.length - 1)) * width * 0.8 + width * 0.1;
            ctx.fillText(point.year, x, height * 0.95);
            ctx.fillText('$' + point.revenue.toFixed(1) + 'M', x, height * 0.05);
        });
        
        // Y-axis labels
        ctx.textAlign = 'right';
        for (let i = 0; i <= 30; i += 5) {
            const y = height * 0.9 - (i / 30) * height * 0.8;
            ctx.fillText('$' + i + 'M', width * 0.08, y + 4);
        }
        
        // Legend
        ctx.textAlign = 'left';
        ctx.fillStyle = '#667eea';
        ctx.fillText('■ Licensing', width * 0.1, height * 0.05);
        ctx.fillStyle = '#764ba2';
        ctx.fillText('■ Performance', width * 0.25, height * 0.05);
        ctx.fillStyle = '#4ade80';
        ctx.fillText('■ SaaS', width * 0.4, height * 0.05);
    }
    
    drawProjectionChart();
}

// Patent demo functionality
function initPatentDemos() {
    const demoBtns = document.querySelectorAll('.demo-btn');
    const modal = document.getElementById('demo-modal');
    const modalContent = document.getElementById('modal-demo-content');
    const closeBtn = document.querySelector('.close');
    
    if (!modal || !modalContent || !closeBtn) return;
    
    demoBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            const demoType = this.getAttribute('data-demo');
            showPatentDemo(demoType);
        });
    });
    
    closeBtn.addEventListener('click', function() {
        modal.style.display = 'none';
    });
    
    window.addEventListener('click', function(e) {
        if (e.target === modal) {
            modal.style.display = 'none';
        }
    });
    
    function showPatentDemo(type) {
        let content = '';
        
        switch(type) {
            case 'qfh':
                content = `
                    <h3>Quantum Field Harmonics (QFH) Demo</h3>
                    <div style="background: #000; padding: 2rem; border-radius: 12px; margin: 1rem 0;">
                        <canvas id="qfh-demo-canvas" width="800" height="400"></canvas>
                    </div>
                    <p>This demo shows how QFH analyzes bit transitions in real-time financial data:</p>
                    <ul>
                        <li><strong>Blue line:</strong> Normal oscillation (FLIP state)</li>
                        <li><strong>Yellow spikes:</strong> Rupture events indicating potential instability</li>
                        <li><strong>Green baseline:</strong> NULL_STATE (stability)</li>
                    </ul>
                    <p><em>Note: This is a simplified visualization. The actual implementation processes data at the bit level.</em></p>
                `;
                break;
            case 'qbsa':
                content = `
                    <h3>Quantum Bit State Analysis (QBSA) Demo</h3>
                    <div style="background: #000; padding: 2rem; border-radius: 12px; margin: 1rem 0;">
                        <canvas id="qbsa-demo-canvas" width="800" height="400"></canvas>
                    </div>
                    <p>QBSA validates pattern integrity through correction ratio analysis:</p>
                    <ul>
                        <li><strong>Purple line:</strong> Expected pattern baseline</li>
                        <li><strong>Orange line:</strong> Actual probe indices</li>
                        <li><strong>Red areas:</strong> High correction ratios (pattern degradation)</li>
                    </ul>
                    <p><em>The correction ratio serves as an early warning system for pattern collapse.</em></p>
                `;
                break;
            case 'manifold':
                content = `
                    <h3>Quantum Manifold Optimizer Demo</h3>
                    <div style="background: #000; padding: 2rem; border-radius: 12px; margin: 1rem 0;">
                        <canvas id="manifold-demo-canvas" width="800" height="400"></canvas>
                    </div>
                    <p>The manifold optimizer maps patterns onto Riemannian surfaces:</p>
                    <ul>
                        <li><strong>3D surface:</strong> Coherence-stability-entropy manifold</li>
                        <li><strong>Red dots:</strong> Current pattern positions</li>
                        <li><strong>Green path:</strong> Optimization trajectory</li>
                    </ul>
                    <p><em>This geometric approach avoids local minima that trap traditional optimizers.</em></p>
                `;
                break;
            case 'evolution':
                content = `
                    <h3>Pattern Evolution System Demo</h3>
                    <div style="background: #000; padding: 2rem; border-radius: 12px; margin: 1rem 0;">
                        <canvas id="evolution-demo-canvas" width="800" height="400"></canvas>
                    </div>
                    <p>Pattern evolution applies genetic algorithms to trading strategies:</p>
                    <ul>
                        <li><strong>Multiple lines:</strong> Different pattern generations</li>
                        <li><strong>Brightness:</strong> Pattern fitness (trading performance)</li>
                        <li><strong>Convergence:</strong> Evolution toward optimal strategies</li>
                    </ul>
                    <p><em>Patterns inherit properties and mutate to discover better trading approaches.</em></p>
                `;
                break;
        }
        
        modalContent.innerHTML = content;
        modal.style.display = 'block';
        
        // Initialize the specific demo
        setTimeout(() => {
            initSpecificDemo(type);
        }, 100);
    }
    
    function initSpecificDemo(type) {
        const canvas = document.getElementById(`${type}-demo-canvas`);
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        const width = canvas.width;
        const height = canvas.height;
        
        // Set up canvas for high DPI displays
        const dpr = window.devicePixelRatio || 1;
        canvas.width = width * dpr;
        canvas.height = height * dpr;
        ctx.scale(dpr, dpr);
        
        switch(type) {
            case 'qfh':
                animateQFHDemo(ctx, width, height);
                break;
            case 'qbsa':
                animateQBSADemo(ctx, width, height);
                break;
            case 'manifold':
                animateManifoldDemo(ctx, width, height);
                break;
            case 'evolution':
                animateEvolutionDemo(ctx, width, height);
                break;
        }
    }
    
    function animateQFHDemo(ctx, width, height) {
        let time = 0;
        const data = [];
        
        function animate() {
            time += 0.05;
            
            // Clear canvas
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, width, height);
            
            // Generate QFH data
            const x = (time % 1) * width;
            const baseValue = height / 2;
            const flip = baseValue + 50 * Math.sin(time * 5);
            const rupture = Math.random() > 0.95 ? baseValue + 100 * (Math.random() - 0.5) : null;
            
            data.push({ x, flip, rupture });
            if (data.length > 200) data.shift();
            
            // Draw NULL_STATE baseline
            ctx.strokeStyle = '#4ade80';
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(0, baseValue);
            ctx.lineTo(width, baseValue);
            ctx.stroke();
            
            // Draw FLIP oscillations
            ctx.strokeStyle = '#667eea';
            ctx.lineWidth = 2;
            ctx.beginPath();
            data.forEach((point, index) => {
                if (index === 0) {
                    ctx.moveTo(point.x, point.flip);
                } else {
                    ctx.lineTo(point.x, point.flip);
                }
            });
            ctx.stroke();
            
            // Draw RUPTURE events
            ctx.fillStyle = '#fbbf24';
            data.forEach(point => {
                if (point.rupture !== null) {
                    ctx.beginPath();
                    ctx.arc(point.x, point.rupture, 5, 0, Math.PI * 2);
                    ctx.fill();
                }
            });
            
            if (time < 20) {
                requestAnimationFrame(animate);
            }
        }
        
        animate();
    }
    
    function animateQBSADemo(ctx, width, height) {
        let time = 0;
        const data = [];
        
        function animate() {
            time += 0.02;
            
            // Clear canvas
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, width, height);
            
            // Generate QBSA data
            for (let i = 0; i < width; i += 5) {
                const expected = height * 0.7 + 30 * Math.sin(i * 0.01);
                const actual = expected + 20 * Math.sin(i * 0.02 + time) + 10 * Math.random();
                const correction = Math.abs(actual - expected) / 50;
                
                data.push({ x: i, expected, actual, correction });
            }
            
            if (data.length > 400) data.splice(0, 200);
            
            // Draw expected pattern
            ctx.strokeStyle = '#a855f7';
            ctx.lineWidth = 2;
            ctx.beginPath();
            data.forEach((point, index) => {
                if (index === 0) {
                    ctx.moveTo(point.x, point.expected);
                } else {
                    ctx.lineTo(point.x, point.expected);
                }
            });
            ctx.stroke();
            
            // Draw actual probe indices
            ctx.strokeStyle = '#f97316';
            ctx.lineWidth = 2;
            ctx.beginPath();
            data.forEach((point, index) => {
                if (index === 0) {
                    ctx.moveTo(point.x, point.actual);
                } else {
                    ctx.lineTo(point.x, point.actual);
                }
            });
            ctx.stroke();
            
            // Draw correction ratio areas
            data.forEach(point => {
                if (point.correction > 0.5) {
                    ctx.fillStyle = `rgba(239, 68, 68, ${point.correction * 0.5})`;
                    ctx.fillRect(point.x - 2, 0, 4, height);
                }
            });
            
            if (time < 15) {
                requestAnimationFrame(animate);
            }
        }
        
        animate();
    }
    
    function animateManifoldDemo(ctx, width, height) {
        let time = 0;
        const patterns = [];
        
        // Initialize patterns
        for (let i = 0; i < 20; i++) {
            patterns.push({
                coherence: Math.random(),
                stability: Math.random(),
                entropy: Math.random(),
                x: Math.random() * width,
                y: Math.random() * height,
                targetX: Math.random() * width,
                targetY: Math.random() * height
            });
        }
        
        function animate() {
            time += 0.02;
            
            // Clear canvas
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, width, height);
            
            // Draw manifold surface (simplified 2D projection)
            for (let x = 0; x < width; x += 20) {
                for (let y = 0; y < height; y += 20) {
                    const value = 0.5 + 0.3 * Math.sin(x * 0.01 + time) * Math.cos(y * 0.01 + time);
                    ctx.fillStyle = `rgba(102, 126, 234, ${value * 0.3})`;
                    ctx.fillRect(x, y, 18, 18);
                }
            }
            
            // Update and draw patterns
            patterns.forEach(pattern => {
                // Move toward optimization target
                pattern.x += (pattern.targetX - pattern.x) * 0.02;
                pattern.y += (pattern.targetY - pattern.y) * 0.02;
                
                // Update target occasionally
                if (Math.random() > 0.99) {
                    pattern.targetX = Math.random() * width;
                    pattern.targetY = Math.random() * height;
                }
                
                // Draw pattern
                ctx.fillStyle = '#ef4444';
                ctx.beginPath();
                ctx.arc(pattern.x, pattern.y, 4, 0, Math.PI * 2);
                ctx.fill();
                
                // Draw optimization path
                ctx.strokeStyle = 'rgba(74, 222, 128, 0.5)';
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(pattern.x, pattern.y);
                ctx.lineTo(pattern.targetX, pattern.targetY);
                ctx.stroke();
            });
            
            if (time < 20) {
                requestAnimationFrame(animate);
            }
        }
        
        animate();
    }
    
    function animateEvolutionDemo(ctx, width, height) {
        let time = 0;
        const generations = [];
        
        // Initialize generations
        for (let g = 0; g < 5; g++) {
            const generation = [];
            for (let i = 0; i < 50; i++) {
                generation.push({
                    x: (i / 49) * width,
                    y: height / 2 + 100 * Math.sin(i * 0.1 + g) + 50 * Math.random(),
                    fitness: Math.random() * 0.5 + g * 0.1
                });
            }
            generations.push(generation);
        }
        
        function animate() {
            time += 0.03;
            
            // Clear canvas
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, width, height);
            
            // Draw each generation
            generations.forEach((generation, genIndex) => {
                const alpha = 0.2 + (genIndex / generations.length) * 0.8;
                
                generation.forEach(pattern => {
                    // Evolve pattern
                    pattern.y += Math.sin(time + pattern.x * 0.01) * 0.5;
                    pattern.fitness = Math.min(1, pattern.fitness + 0.001);
                });
                
                // Draw generation line
                ctx.strokeStyle = `rgba(102, 126, 234, ${alpha * pattern.fitness})`;
                ctx.lineWidth = 1 + genIndex;
                ctx.beginPath();
                generation.forEach((pattern, index) => {
                    if (index === 0) {
                        ctx.moveTo(pattern.x, pattern.y);
                    } else {
                        ctx.lineTo(pattern.x, pattern.y);
                    }
                });
                ctx.stroke();
            });
            
            if (time < 15) {
                requestAnimationFrame(animate);
            }
        }
        
        animate();
    }
}

// Scroll animations
function initScrollAnimations() {
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };
    
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add('fade-in-up');
            }
        });
    }, observerOptions);
    
    // Observe all tech cards, performance metrics, etc.
    const animatedElements = document.querySelectorAll('.tech-card, .problem-item, .study-item, .patent-item, .highlight-item');
    animatedElements.forEach(el => observer.observe(el));
}

// Utility functions
function debounce(func, wait) {
    let timeout;
    return function executedFunction(...args) {
        const later = () => {
            clearTimeout(timeout);
            func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
    };
}

// Handle window resize
window.addEventListener('resize', debounce(() => {
    // Reinitialize charts on resize
    initPerformanceChart();
    initProjectionChart();
}, 250));
