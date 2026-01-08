#pragma once
#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Water Leak Detector - ESP32</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        /* ---------- Rainbow button variables ---------- */
        :root {
          --bg-position: 100;
          --after-blur: 10;
          --after-opacity: 1;
          --before-opacity: 0.3;
          --btn-offset: 1;
          --btn-scale: 1;
          --btn-border-width: 1.5;
          --after-pos-y: 10;

          --btn-bg: hsl(0, 0%, 0%);

          --color-white: hsl(0, 0%, 100%);
          --color-cyan: hsl(180, 100%, 50%);
          --color-blue: hsl(240, 100%, 50%);
          --color-purple: hsl(270, 100%, 50%);
          --color-pink: hsl(330, 40%, 70%);
          --color-red: hsl(0, 100%, 50%);
          --color-yellow: hsl(60, 100%, 50%);
          --color-lime: hsl(90, 100%, 75%);
        }

        body {
            font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
            background: radial-gradient(circle at top left, #111827 0%, #020617 40%, #000000 100%);
            min-height: 100vh;
            padding: 20px;
            color: #e5e7eb;
            transition: background 0.4s ease;
        }

        /* Alert mode background */
        body.alert {
            background: radial-gradient(circle at top left, #4c0519 0%, #111827 40%, #020617 100%);
        }

        .main-container {
            max-width: 1400px;
            margin: 0 auto;
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
            animation: fadeIn 0.5s ease-in;
        }

        @keyframes fadeIn {
            from {
                opacity: 0;
                transform: translateY(-20px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .container {
            background: #020617;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.7);
            padding: 40px;
            width: 100%;
            border: 1px solid #1f2937;
            transition: box-shadow 0.4s ease, border-color 0.4s ease;
        }

        .graph-container {
            background: #020617;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.7);
            padding: 40px;
            width: 100%;
            display: flex;
            flex-direction: column;
            border: 1px solid #1f2937;
        }

        h1 {
            color: #f9fafb;
            text-align: center;
            margin-bottom: 10px;
            font-size: 2em;
        }

        .subtitle {
            text-align: center;
            color: #9ca3af;
            margin-bottom: 30px;
            font-size: 0.9em;
        }

        .graph-title {
            color: #f9fafb;
            text-align: center;
            margin-bottom: 20px;
            font-size: 1.5em;
        }

        .status-card {
            background: radial-gradient(circle at top left, #4f46e5, #7c3aed);
            border-radius: 15px;
            padding: 30px;
            margin-bottom: 25px;
            text-align: center;
            color: white;
            box-shadow: 0 10px 30px rgba(88, 80, 236, 0.5);
        }

        .status-label {
            font-size: 0.9em;
            opacity: 0.9;
            margin-bottom: 10px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        .status-value {
            font-size: 3em;
            font-weight: bold;
            margin-bottom: 10px;
        }

        .status-unit {
            font-size: 1.2em;
            opacity: 0.8;
        }

        .alarm-status {
            background: #020617;
            border-radius: 15px;
            padding: 25px;
            margin-bottom: 25px;
            text-align: center;
            border: 1px solid #1f2937;
        }

        .alarm-indicator {
            display: inline-block;
            width: 16px;
            height: 16px;
            border-radius: 50%;
            margin-right: 10px;
            animation: pulse 2s infinite;
        }

        .alarm-active {
            background: #ef4444;
            box-shadow: 0 0 20px rgba(248, 113, 113, 0.8);
        }

        .alarm-inactive {
            background: #22c55e;
            animation: none;
        }

        @keyframes pulse {
            0%, 100% {
                opacity: 1;
                transform: scale(1);
            }
            50% {
                opacity: 0.7;
                transform: scale(1.1);
            }
        }

        .alarm-text {
            font-size: 1.3em;
            font-weight: bold;
            color: #f9fafb;
        }

        .alarm-time {
            margin-top: 10px;
            color: #9ca3af;
            font-size: 0.9em;
        }

        .info-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
            margin-bottom: 25px;
        }

        .info-card {
            background: #020617;
            border-radius: 12px;
            padding: 20px;
            text-align: center;
            border: 1px solid #1f2937;
        }

        .info-label {
            font-size: 0.8em;
            color: #9ca3af;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            margin-bottom: 8px;
        }

        .info-value {
            font-size: 1.5em;
            font-weight: bold;
            color: #e5e7eb;
        }
        .progress-bar {
            background: #111827;
            border-radius: 10px;
            height: 30px;
            margin: 20px 0;
            overflow: hidden;
            position: relative;   /* <-- important */
        }

        .progress-fill {
            background: linear-gradient(90deg, #22c55e, #06b6d4);
            height: 100%;
            border-radius: 10px;
            transition: width 0.3s ease, background 0.3s ease;
            display: block;       /* no need to center content anymore */
        }

        /* NEW */
        .progress-label {
            position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%, -50%);
            color: white;
            font-weight: bold;
            font-size: 0.9em;
            pointer-events: none;  /* clicks go through */
        }

        .progress-fill.warning {
            background: linear-gradient(90deg, #f97316, #facc15);
        }

        .progress-fill.danger {
            background: linear-gradient(90deg, #ef4444, #f97316);
        }

        .footer {
            text-align: center;
            color: #6b7280;
            font-size: 0.85em;
            margin-top: 20px;
        }

        .last-update {
            color: #4b5563;
            font-size: 0.8em;
            margin-top: 10px;
        }

        /* Simple dark pill button for reset */

        .button-row {
          margin-top: 18px;
          display: flex;
          gap: 12px;
          justify-content: center;
          flex-wrap: wrap;
        }

        
        #sensorGraph {
            width: 100%;
            height: 400px;
            border: 1px solid #1f2937;
            border-radius: 10px;
            background: #020617;
        }

        .graph-stats {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            gap: 10px;
            margin-top: 15px;
        }

        .graph-stat {
            background: #020617;
            border-radius: 8px;
            padding: 12px;
            text-align: center;
            border: 1px solid #1f2937;
        }

        .graph-stat-label {
            font-size: 0.75em;
            color: #9ca3af;
            text-transform: uppercase;
            margin-bottom: 5px;
        }

        .graph-stat-value {
            font-size: 1.2em;
            font-weight: bold;
            color: #e5e7eb;
        }

        @media (max-width: 1200px) {
            .main-container {
                grid-template-columns: 1fr;
            }
        }

        @media (max-width: 600px) {
            .container, .graph-container {
                padding: 25px;
            }

            h1 {
                font-size: 1.5em;
            }

            .status-value {
                font-size: 2em;
            }

            .info-grid {
                grid-template-columns: 1fr;
            }

            #sensorGraph {
                height: 300px;
            }

            .graph-stats {
                grid-template-columns: 1fr;
            }
        }


        button.rainbow-btn {
          all: unset;
          background: transparent;
          border-width: 0;
          transform: scale(var(--btn-scale));
          cursor: pointer;
          margin-top: 15px;
          margin-left: 10px;
          transition:
            --bg-position 3s ease,
            --after-blur 0.3s ease,
            --before-opacity 0.3s ease,
            --btn-offset 0.3s ease,
            --btn-scale 0.2s cubic-bezier(.76,-0.25,.51,1.13);
        }

        button.rainbow-btn > div {
          display: block;
          padding: 0.7em 1.4em;
          background: var(--btn-bg);
          color: white;
          font-weight: bold;
          border-radius: 8px;
          font-size: 16px;
          position: relative;
          cursor: pointer;
        }

        button.rainbow-btn > div > span {
          background: linear-gradient(
              to right,
              var(--color-white),
              var(--color-white),
              var(--color-cyan),
              var(--color-blue),
              var(--color-purple),
              var(--color-pink),
              var(--color-red),
              var(--color-yellow),
              var(--color-lime),
              var(--color-white),
              var(--color-white)
            )
            no-repeat calc(var(--bg-position) * 1%) 0% / 900%;
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
          letter-spacing: 0.12ch;
          font-weight: 600;
        }

        button.rainbow-btn > div::after {
          display: block;
          position: absolute;
          content: "";
          width: 100%;
          height: 100%;
          background: linear-gradient(
              to right,
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0),
              rgb(0 0 0)
            )
            no-repeat calc(var(--bg-position) * 1%) 0% / 900%;
          transform: translateY(calc(var(--after-pos-y) * 1px));
          left: 0;
          top: 0;
          z-index: -2;
          filter: blur(calc(var(--after-blur) * 1px));
          opacity: var(--after-opacity);
        }

        button.rainbow-btn > div::before {
          content: "";
          display: block;
          position: absolute;
          width: calc(100% + calc(calc(var(--btn-border-width) * 2) * 1px));
          height: calc(100% + calc(calc(var(--btn-border-width) * 2) * 1px));
          background: linear-gradient(
              to right,
              var(--color-white),
              var(--color-white),
              var(--color-cyan),
              var(--color-blue),
              var(--color-purple),
              var(--color-pink),
              var(--color-red),
              var(--color-yellow),
              var(--color-lime),
              var(--color-white),
              var(--color-white)
            )
            no-repeat calc(var(--bg-position) * 1%) 0% / 900%;
          border-radius: 9px;
          z-index: -1;
          top: calc(var(--btn-border-width) * -1px);
          left: calc(var(--btn-border-width) * -1px);
          opacity: var(--before-opacity);
        }

        button.rainbow-btn:hover {
          --btn-scale: 1.05;
          --bg-position: 0;
          --after-blur: 30;
          --after-opacity: 0.3;
          --after-pos-y: 0;
          --before-opacity: 1;
          --btn-offset: 5;
        }

        button.rainbow-btn:active {
          --btn-scale: 0.98;
          --after-blur: 15;
        }
    </style>
</head>
<body id="pageBody">
    <div class="main-container">
        <div class="container">
            <h1>💧 Water Leak Detector</h1>
            <p class="subtitle">ESP32 Monitoring System</p>

            <div class="status-card">
                <div class="status-label">Sensor Reading</div>
                <div class="status-value" id="sensorValue">--</div>
                <div class="status-unit">Analog Value</div>
            </div>

            <div class="progress-bar">
                <div class="progress-fill" id="progressBar" style="width: 0%">
                    <span id="progressText">0%</span>
                </div>
            </div>

            </div>
            
            <div class="alarm-status">
                <div>
                    <span class="alarm-indicator" id="alarmIndicator"></span>
                    <span class="alarm-text" id="alarmText">System Normal</span>
                </div>
                <div class="alarm-time" id="alarmTime"></div>

                <div class="button-row">
                    <button class="rainbow-btn" onclick="resetAlarm()">
                      <div><span>RESET ALARM</span></div>
                    </button>

                    <button class="rainbow-btn" onclick="testNotify()">
                      <div><span>TEST NOTIFICATION</span></div>
                    </button>
                </div>
            </div>


            <div class="info-grid">
                <div class="info-card">
                    <div class="info-label">Threshold</div>
                    <div class="info-value" id="thresholdValue">1000</div>
                </div>
                <div class="info-card">
                    <div class="info-label">Status</div>
                    <div class="info-value" id="systemStatus">Monitoring</div>
                </div>
            </div>

            <div class="footer">
                <div>ESP32 Water Leak Detection System</div>
                <div class="last-update" id="lastUpdate">Last update: --</div>
            </div>
        </div>

        <div class="graph-container">
            <h2 class="graph-title">📊 Live Sensor Graph</h2>

            <!-- Legend -->
            <div style="display:flex; justify-content:flex-end; font-size:0.8em; color:#9ca3af; margin-bottom:8px;">
                <div style="margin-left:10px;">
                    <span style="display:inline-block;width:10px;height:10px;border-radius:50%;background:#667eea;margin-right:5px;"></span>
                    Sensor value
                </div>
                <div style="margin-left:10px;">
                    <span style="display:inline-block;width:10px;height:10px;border-radius:50%;background:#ffc107;margin-right:5px;"></span>
                    Threshold
                </div>
            </div>

            <canvas id="sensorGraph"></canvas>
            <div class="graph-stats">
                <div class="graph-stat">
                    <div class="graph-stat-label">Min</div>
                    <div class="graph-stat-value" id="graphMin">--</div>
                </div>
                <div class="graph-stat">
                    <div class="graph-stat-label">Max</div>
                    <div class="graph-stat-value" id="graphMax">--</div>
                </div>
                <div class="graph-stat">
                    <div class="graph-stat-label">Avg</div>
                    <div class="graph-stat-value" id="graphAvg">--</div>
                </div>
            </div>
        </div>
    </div>

    <script>
        const threshold = 1000;
        const maxValue = 4095; // ESP32 ADC max value
        
        // Graph configuration
        const MAX_DATA_POINTS = 60; // Show last 60 data points (60 seconds if updating every second)
        const graphData = [];
        let canvas, ctx;
        
        // Initialize canvas
        function initGraph() {
            canvas = document.getElementById('sensorGraph');
            ctx = canvas.getContext('2d');
            
            // Set canvas size
            const rect = canvas.getBoundingClientRect();
            canvas.width = rect.width;
            canvas.height = rect.height;
            
            // Draw initial graph
            drawGraph();
        }
        
        // Draw the graph
        function drawGraph() {
            if (!ctx || graphData.length === 0) return;
            
            const width = canvas.width;
            const height = canvas.height;
            const padding = 40;
            const graphWidth = width - 2 * padding;
            const graphHeight = height - 2 * padding;
            
            // Clear canvas
            ctx.clearRect(0, 0, width, height);
            
            // Draw background grid
            ctx.strokeStyle = 'rgba(148, 163, 184, 0.2)';
            ctx.lineWidth = 1;
            
            // Horizontal grid lines
            for (let i = 0; i <= 5; i++) {
                const y = padding + (graphHeight / 5) * i;
                ctx.beginPath();
                ctx.moveTo(padding, y);
                ctx.lineTo(width - padding, y);
                ctx.stroke();
            }
            
            // Vertical grid lines
            for (let i = 0; i <= 10; i++) {
                const x = padding + (graphWidth / 10) * i;
                ctx.beginPath();
                ctx.moveTo(x, padding);
                ctx.lineTo(x, height - padding);
                ctx.stroke();
            }
            
            // Find min and max values for scaling
            const values = graphData.map(d => d.value);
            const minVal = Math.min(...values, 0);
            const maxVal = Math.max(...values, maxValue);
            const range = maxVal - minVal || 1;
            
            // Draw threshold line
            const thresholdY = padding + graphHeight - ((threshold - minVal) / range) * graphHeight;
            ctx.strokeStyle = '#facc15';
            ctx.lineWidth = 2;
            ctx.setLineDash([5, 5]);
            ctx.beginPath();
            ctx.moveTo(padding, thresholdY);
            ctx.lineTo(width - padding, thresholdY);
            ctx.stroke();
            ctx.setLineDash([]);
            
            // Draw threshold label
            ctx.fillStyle = '#facc15';
            ctx.font = '12px sans-serif';
            ctx.fillText('Threshold: ' + threshold, width - padding - 100, thresholdY - 5);
            
            // Draw the line graph
            if (graphData.length > 1) {
                ctx.strokeStyle = '#60a5fa';
                ctx.lineWidth = 3;
                ctx.beginPath();
                
                graphData.forEach((point, index) => {
                    const x = padding + (graphWidth / (MAX_DATA_POINTS - 1)) * index;
                    const y = padding + graphHeight - ((point.value - minVal) / range) * graphHeight;
                    
                    if (index === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        ctx.lineTo(x, y);
                    }
                });
                
                ctx.stroke();
                
                // Draw filled area under the line
                ctx.fillStyle = 'rgba(37, 99, 235, 0.18)';
                ctx.lineTo(width - padding, height - padding);
                ctx.lineTo(padding, height - padding);
                ctx.closePath();
                ctx.fill();
                
                // Draw data points
                ctx.fillStyle = '#60a5fa';
                graphData.forEach((point, index) => {
                    const x = padding + (graphWidth / (MAX_DATA_POINTS - 1)) * index;
                    const y = padding + graphHeight - ((point.value - minVal) / range) * graphHeight;
                    
                    ctx.beginPath();
                    ctx.arc(x, y, 4, 0, 2 * Math.PI);
                    ctx.fill();
                    
                    // Highlight points above threshold
                    if (point.value >= threshold) {
                        ctx.fillStyle = '#ef4444';
                        ctx.beginPath();
                        ctx.arc(x, y, 6, 0, 2 * Math.PI);
                        ctx.fill();
                        ctx.fillStyle = '#60a5fa';
                    }
                });
            }
            
            // Draw Y-axis labels
            ctx.fillStyle = '#9ca3af';
            ctx.font = '11px sans-serif';
            ctx.textAlign = 'right';
            for (let i = 0; i <= 5; i++) {
                const value = minVal + (range / 5) * (5 - i);
                const y = padding + (graphHeight / 5) * i;
                ctx.fillText(Math.round(value), padding - 10, y + 4);
            }
            
            // Draw X-axis labels (time)
            ctx.textAlign = 'center';
            const timeLabels = 5;
            for (let i = 0; i <= timeLabels; i++) {
                const index = Math.floor((graphData.length - 1) * (i / timeLabels));
                if (index >= 0 && index < graphData.length) {
                    const x = padding + (graphWidth / timeLabels) * i;
                    const minutes = Math.floor(graphData[index].time / 60);
                    const seconds = graphData[index].time % 60;
                    ctx.fillText(`${minutes}:${seconds.toString().padStart(2, '0')}`, x, height - padding + 20);
                }
            }
            
            // Update graph statistics
            updateGraphStats(values);
        }
        
        function updateGraphStats(values) {
            if (values.length === 0) return;
            
            const min = Math.min(...values);
            const max = Math.max(...values);
            const avg = Math.round(values.reduce((a, b) => a + b, 0) / values.length);
            
            document.getElementById('graphMin').textContent = min;
            document.getElementById('graphMax').textContent = max;
            document.getElementById('graphAvg').textContent = avg;
        }
        
        function addDataPoint(value) {
            const time = graphData.length; // Simple time index (seconds)
            graphData.push({ value, time });
            
            // Keep only last MAX_DATA_POINTS
            if (graphData.length > MAX_DATA_POINTS) {
                graphData.shift();
                // Adjust time indices
                graphData.forEach((point, index) => {
                    point.time = index;
                });
            }
            
            drawGraph();
        }

        async function testNotify() {
            try {
                await fetch('/test_notify');
            } catch (e) {
                console.error('Error sending test notification:', e);
            }
        }

        function updateDisplay(data) {
            const sensorValue = data.value ?? 0;
            const isAlarm = data.leak ?? false;

            // Add data point to graph
            addDataPoint(sensorValue);

            // Update sensor value
            document.getElementById('sensorValue').textContent = sensorValue;

            // Update progress bar
            const percentage = Math.min((sensorValue / maxValue) * 100, 100);
            const progressBar = document.getElementById('progressBar');
            const progressText = document.getElementById('progressText');
            
            progressBar.style.width = percentage + '%';
            progressText.textContent = Math.round(percentage) + '%';

            // Reset classes then apply based on threshold
            progressBar.className = 'progress-fill';
            if (sensorValue >= threshold) {
                progressBar.classList.add('danger');
            } else if (sensorValue >= threshold * 0.7) {
                progressBar.classList.add('warning');
            }

            // Update alarm status
            const alarmIndicator = document.getElementById('alarmIndicator');
            const alarmText = document.getElementById('alarmText');
            const alarmTime = document.getElementById('alarmTime');
            const systemStatus = document.getElementById('systemStatus');
            const container = document.querySelector('.container');
            const body = document.getElementById('pageBody');

            if (isAlarm) {
                alarmIndicator.className = 'alarm-indicator alarm-active';
                alarmText.textContent = '⚠️ LEAK DETECTED!';
                alarmText.style.color = '#f97316';

                alarmTime.textContent = 'Alarm active';
                systemStatus.textContent = 'Alert';
                systemStatus.style.color = '#ef4444';

                container.style.boxShadow = '0 20px 60px rgba(248, 113, 113, 0.6)';
                container.style.borderColor = '#b91c1c';
                body.classList.add('alert');
            } else {
                alarmIndicator.className = 'alarm-indicator alarm-inactive';
                alarmText.textContent = '✓ System Normal';
                alarmText.style.color = '#22c55e';
                alarmTime.textContent = 'No leaks detected';
                systemStatus.textContent = 'Normal';
                systemStatus.style.color = '#22c55e';

                container.style.boxShadow = '0 20px 60px rgba(0, 0, 0, 0.7)';
                container.style.borderColor = '#1f2937';
                body.classList.remove('alert');
            }

            // Update threshold display
            document.getElementById('thresholdValue').textContent = threshold;

            // Update last update time
            const now = new Date();
            document.getElementById('lastUpdate').textContent = `Last update: ${now.toLocaleTimeString()}`;
        }

        // Fetch data from ESP32
        async function fetchData() {
            try {
                const response = await fetch('/data');
                const data = await response.json();
                updateDisplay(data);
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        }

        async function resetAlarm() {
            try {
                await fetch('/reset');
                fetchData();
            } catch (e) {
                console.error('Error resetting alarm:', e);
            }
        }

        // Handle window resize
        window.addEventListener('resize', () => {
            const rect = canvas.getBoundingClientRect();
            canvas.width = rect.width;
            canvas.height = rect.height;
            drawGraph();
        });

        // Initialize graph on load
        window.addEventListener('load', () => {
            initGraph();
            fetchData(); // Initial load
            setInterval(fetchData, 1000); // Update every second
        });
    </script>
</body>
</html>
)rawliteral";


