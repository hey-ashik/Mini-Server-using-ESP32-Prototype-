#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h> 
#include <Wire.h>               
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>  

// --- Display Settings ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1      
#define SCREEN_ADDRESS 0x3C   

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Preferences preferences;
WebServer server(80);

// Default Credentials 
String ssid = "Al Amin Islam Ar";
String password = "ALAMINASR60";

// --- Sleep Timer Variables ---
unsigned long startTime; 
const unsigned long DISPLAY_TIMEOUT = 40000; // 60 seconds in milliseconds
bool isDisplayOn = true;

// --- Main Portfolio HTML (PROGMEM) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ashikul Islam | Portfolio</title>
    <style>
        :root { --bg: #ffffff; --text: #000000; --gray: #666; --border: #eee; }
        * { margin: 0; padding: 0; box-sizing: border-box; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; }
        html { scroll-behavior: smooth; }
        body { background: var(--bg); color: var(--text); line-height: 1.5; }
        nav { position: fixed; top: 0; width: 100%; padding: 1rem 5%; display: flex; justify-content: space-between; align-items: center; background: rgba(255,255,255,0.95); z-index: 100; border-bottom: 1px solid var(--border); }
        .logo { font-weight: 800; font-size: 1.1rem; letter-spacing: -0.5px; }
        .nav-links { display: flex; gap: 1.5rem; }
        .nav-links a { text-decoration: none; color: var(--text); font-size: 0.85rem; font-weight: 500; opacity: 0.7; }
        .menu-btn { display: none; cursor: pointer; font-size: 1.2rem; }
        header { height: 90vh; display: flex; flex-direction: column; justify-content: center; padding: 0 10%; background: #fafafa; }
        h1 { font-size: clamp(2.5rem, 10vw, 5rem); line-height: 0.9; letter-spacing: -2px; margin: 1rem 0; }
        .btn { display: inline-block; padding: 0.8rem 1.5rem; background: #000; color: #fff; text-decoration: none; font-size: 0.9rem; font-weight: 600; margin-top: 1rem; width: fit-content; }
        section { padding: 4rem 10%; border-bottom: 1px solid var(--border); }
        .section-title { font-size: 1.5rem; text-transform: uppercase; letter-spacing: 2px; margin-bottom: 2rem; border-left: 4px solid #000; padding-left: 10px; }
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 2rem; }
        .card { border: 1px solid var(--border); padding: 1.5rem; transition: 0.2s; }
        .card:hover { border-color: #000; }
        .card h3 { margin-bottom: 0.5rem; font-size: 1.2rem; }
        .card p { font-size: 0.9rem; color: var(--gray); margin-bottom: 1rem; }
        .tag { display: inline-block; padding: 0.3rem 0.8rem; border: 1px solid #000; font-size: 0.75rem; margin: 0.2rem; font-weight: 600; }
        .exp-item { margin-bottom: 2rem; }
        .date { font-size: 0.8rem; font-weight: 700; color: var(--gray); }
        ul { list-style: none; margin-top: 0.5rem; }
        ul li { font-size: 0.9rem; color: var(--gray); margin-bottom: 0.3rem; }
        ul li::before { content: "• "; color: #000; }
        footer { padding: 4rem 10%; background: #000; color: #fff; text-align: center; }
        .contact-box { display: flex; justify-content: center; gap: 2rem; flex-wrap: wrap; margin-top: 2rem; }
        .contact-box a { color: #fff; text-decoration: none; display: flex; align-items: center; gap: 0.5rem; font-size: 0.9rem; }
        svg { width: 18px; height: 18px; fill: currentColor; }
        @media (max-width: 768px) {
            .nav-links { display: none; position: absolute; top: 100%; left: 0; width: 100%; background: #fff; flex-direction: column; padding: 1.5rem; border-bottom: 1px solid var(--border); }
            .nav-links.active { display: flex; }
            .menu-btn { display: block; }
            header, section { padding: 4rem 5%; }
        }
    </style>
</head>
<body>
    <nav>
        <div class="logo">ASHiK</div>
        <div class="menu-btn" onclick="toggleMenu()">☰</div>
        <div class="nav-links" id="navLinks">
            <a href="#about">About</a>
            <a href="#experience">Experience</a>
            <a href="#projects">Projects</a>
            <a href="#skills">Skills</a>
            <a href="#contact">Contact</a>
        </div>
    </nav>
    <header>
        <p style="font-weight: 600; font-size: 0.8rem; letter-spacing: 2px;">EEE & AI DEVELOPER</p>
        <h1>ASHIKUL<br>ISLAM</h1>
        <p style="color:var(--gray); max-width: 500px;">B.Sc. in Electrical & Electronics Engineering with a focus on AI-driven solutions and RAG applications.</p>
        <a href="#contact" class="btn">HIRE ME</a>
    </header>
    <section id="experience">
        <h2 class="section-title">Experience</h2>
        <div class="exp-item">
            <span class="date">FEB - NOV 2025</span>
            <h3>Student Associate — DIU BLC</h3>
            <ul>
                <li>Mentored trainees for LMS operations and technical support.</li>
                <li>Resolved 100+ academic/technical issues for students.</li>
            </ul>
        </div>
        <div class="exp-item">
            <span class="date">3 MONTHS</span>
            <h3>Trainee Engineer — BETELCO</h3>
            <ul>
                <li>Specialized in transformer manufacturing and quality testing.</li>
                <li>Collaborated on design, maintenance, and safety procedures.</li>
            </ul>
        </div>
    </section>
    <section id="projects" style="background:#fcfcfc">
        <h2 class="section-title">AI Projects</h2>
        <div class="grid">
            <div class="card">
                <h3>RAG Chat App</h3>
                <p>Advanced AI system built on 120B-parameter architecture with vision search.</p>
                <a href="https://totapakhi.busyashik.com" style="color:#000; font-size:0.8rem; font-weight:700;">LIVE LINK ↗</a>
            </div>
            <div class="card">
                <h3>EduMate LMS</h3>
                <p>AI-enabled learning platform serving 20,000+ worldwide users.</p>
                <a href="https://edumate.ashikone.com" style="color:#000; font-size:0.8rem; font-weight:700;">LIVE LINK ↗</a>
            </div>
        </div>
    </section>
    <section id="skills">
        <h2 class="section-title">Technical Skills</h2>
        <div style="margin-bottom: 2rem;">
            <p style="font-weight: 700; margin-bottom: 0.5rem;">AI & SOFTWARE</p>
            <span class="tag">Python</span> <span class="tag">LangChain</span> <span class="tag">Next.js</span> <span class="tag">WordPress</span> <span class="tag">AI-Tools</span>
        </div>
        <div>
            <p style="font-weight: 700; margin-bottom: 0.5rem;">ENGINEERING</p>
            <span class="tag">PSpice</span> <span class="tag">Circuit Design</span> <span class="tag">Wiring</span> <span class="tag">Power Systems</span>
        </div>
    </section>
    <section id="education">
        <h2 class="section-title">Education</h2>
        <div class="exp-item">
            <h3>B.Sc in EEE</h3>
            <p>Daffodil International University | CGPA: 3.93/4.00</p>
        </div>
        <div class="exp-item">
            <h3>Diploma in Electrical</h3>
            <p>Chittagong Polytechnic | CGPA: 3.36/4.00</p>
        </div>
    </section>
    <footer id="contact">
        <h2>GET IN TOUCH</h2>
        <div class="contact-box">
            <a href="mailto:ashikulislam2070@gmail.com">
                <svg viewBox="0 0 24 24"><path d="M20 4H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V6c0-1.1-.9-2-2-2zm0 4l-8 5-8-5V6l8 5 8-5v2z"/></svg>
                Email
            </a>
            <a href="https://linkedin.com/in/ashikulislamm">
                <svg viewBox="0 0 24 24"><path d="M19 3a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h14m-.5 15.5v-5.3a3.26 3.26 0 0 0-3.26-3.26c-.85 0-1.84.52-2.32 1.3v-1.11h-2.79v8.37h2.79v-4.93c0-.77.62-1.4 1.39-1.4a1.4 1.4 0 0 1 1.4 1.4v4.93h2.79M6.88 8.56a1.68 1.68 0 0 0 1.68-1.68c0-.93-.75-1.69-1.68-1.69a1.69 1.69 0 0 0-1.69 1.69c0 .93.76 1.68 1.69 1.68m1.39 9.94v-8.37H5.5v8.37h2.77z"/></svg>
                LinkedIn
            </a>
        </div>
        <p style="margin-top: 3rem; opacity: 0.4; font-size: 0.7rem;">&copy; 2026 ASHIKUL ISLAM</p>
    </footer>
    <script>
        function toggleMenu() { document.getElementById('navLinks').classList.toggle('active'); }
        document.querySelectorAll('.nav-links a').forEach(l => {
            l.addEventListener('click', () => document.getElementById('navLinks').classList.remove('active'));
        });
    </script>
</body>
</html>
)rawliteral";

const char admin_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Admin Panel</title>
    <style>
        body { font-family: sans-serif; display: flex; justify-content: center; background: #f0f0f0; padding: 20px; }
        .login-card { background: white; padding: 30px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); width: 100%; max-width: 350px; }
        input { width: 100%; padding: 12px; margin: 10px 0; border: 1px solid #ddd; border-radius: 6px; }
        button { width: 100%; padding: 12px; background: #000; color: #fff; border: none; border-radius: 6px; cursor: pointer; font-weight: bold; }
        h2 { margin-bottom: 20px; text-align: center; }
    </style>
</head>
<body>
    <div class="login-card">
        <h2>WiFi Settings</h2>
        <form action="/save" method="POST">
            <label>New SSID:</label>
            <input type="text" name="n_ssid" placeholder="WiFi Name" required>
            <label>New Password:</label>
            <input type="password" name="n_pass" placeholder="Password" required>
            <button type="submit">Save & Restart</button>
        </form>
    </div>
</body>
</html>
)rawliteral";

void handleRoot() {
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    server.sendContent(index_html);
    server.sendContent("");
    server.client().stop();
}

void handleAdmin() {
    server.send_P(200, "text/html", admin_html);
}

void handleSave() {
    if (server.hasArg("n_ssid") && server.hasArg("n_pass")) {
        String newSsid = server.arg("n_ssid");
        String newPass = server.arg("n_pass");
        preferences.begin("wifi-creds", false);
        preferences.putString("ssid", newSsid);
        preferences.putString("pass", newPass);
        preferences.end();
        server.send(200, "text/html", "<h2>Settings Saved!</h2><p>ESP32 is restarting...</p>");
        delay(3000);
        ESP.restart();
    }
}

void animateEyes(int loops) {
    for(int i=0; i<loops; i++) {
        display.clearDisplay();
        display.fillRect(20, 15, 30, 30, SSD1306_WHITE); 
        display.fillRect(78, 15, 30, 30, SSD1306_WHITE); 
        display.display();
        delay(800);
        display.clearDisplay();
        display.fillRect(20, 28, 30, 4, SSD1306_WHITE); 
        display.fillRect(78, 28, 30, 4, SSD1306_WHITE); 
        display.display();
        delay(200);
    }
    display.clearDisplay();
    display.fillRect(20, 15, 30, 30, SSD1306_WHITE); 
    display.fillRect(78, 15, 30, 30, SSD1306_WHITE); 
    display.display();
    delay(500);
}

void typeWrite(String text, int x, int y, int size) {
    display.setTextSize(size);
    display.setCursor(x, y);
    for(int i=0; i < text.length(); i++) {
        display.print(text[i]);
        display.display();
        delay(50);
    }
}

void setup() {
    Serial.begin(115200);

    Wire.begin(21, 22); 
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    display.ssd1306_command(SSD1306_DISPLAYON); // Ensure display is on at start
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); 
    
    animateEyes(2); 
    display.clearDisplay();
    delay(500);

    typeWrite("Hello", 10, 10, 2); 
    typeWrite("Everyone!", 10, 35, 2);
    delay(1500);
    
    display.clearDisplay();
    typeWrite("Mini", 0, 0, 2);
    typeWrite("WEB Server", 0, 20, 2);
    typeWrite("ProtoType", 0, 45, 1);
    delay(2000);

    preferences.begin("wifi-creds", true);
    ssid = preferences.getString("ssid", ssid); 
    password = preferences.getString("pass", password);
    preferences.end();

    WiFi.begin(ssid.c_str(), password.c_str());
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Connecting Wifi...");
    display.display();

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        display.print(".");
        display.display();
        timeout++;
    }

    if (MDNS.begin("ashik")) {
        Serial.println("\nmDNS started: http://ashik.local");
    }

    server.on("/", handleRoot);
    server.on("/admin", handleAdmin);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    MDNS.addService("http", "tcp", 80);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("WiFi Info");
    display.drawLine(0, 18, 128, 18, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("SSID: ");
    display.println(ssid); 
    display.setCursor(0, 40);
    display.print("PASS: ");
    display.println(password); 
    display.display(); 
    delay(4000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Server ON");
    display.drawLine(0, 18, 128, 18, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.setCursor(0, 40);
    display.print("Visit: ");
    display.println("ashik.local");
    display.display(); 

    // Record the time when the setup finished
    startTime = millis();
}

void loop() {
    server.handleClient();

    // Check if 60 seconds have passed since setup ended
    if (isDisplayOn && (millis() - startTime > DISPLAY_TIMEOUT)) {
        display.clearDisplay();
        display.display();
        display.ssd1306_command(SSD1306_DISPLAYOFF); // Send OLED to low-power sleep
        isDisplayOn = false;
        Serial.println("Display entered sleep mode to save battery.");
    }
}
