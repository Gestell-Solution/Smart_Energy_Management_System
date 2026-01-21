# 🚀 How to Deploy Your Dashboard to the Web

This guide explains how to deploy your Smart Energy Dashboard so you can access it from anywhere (Laptop, Mobile, etc.) without needing it running on your local computer.

## ✅ Option 1: Render (Easiest & Free)

We recommend **Render** because it has a generous free tier for Node.js apps and requires zero configuration.

### Prerequisites

1.  A **GitHub Account**.
2.  Your project code pushed to a GitHub repository.

### Steps

1.  **Login to Render**: Go to [render.com](https://render.com/) and sign up with GitHub.
2.  **New Web Service**:
    - Click the "New +" button.
    - Select **Web Service**.
3.  **Connect Repo**:
    - Find your repository in the list (you may need to give permission to Render).
    - Click **Connect**.
4.  **Configure**:
    - **Name**: Choose a name (e.g., `my-energy-dashboard`).
    - **Region**: Select the one closest to you (e.g., Frankfurt/London).
    - **Branch**: `main` (or master).
    - **Root Directory**: `Firmware/Dashboard/Src/Dashboard` (⚠️ **Important**: Point this to where your `package.json` is).
    - **Runtime**: `Node`
    - **Build Command**: `npm install` (default is fine).
    - **Start Command**: `node server.js` (default may be `npm start`, which is also fine).
5.  **Deploy**:
    - Click **Create Web Service**.
    - Wait for the build to finish (about 1-2 minutes).
6.  **Done!**
    - You will get a URL like `https://my-energy-dashboard.onrender.com`.
    - Open this URL on your phone or laptop.

---

## 🌩️ Option 2: Railway (Alternative)

**Railway** is also excellent but gives you a trial credit (still enough for months of testing).

1.  Go to [railway.app](https://railway.app/).
2.  Login with GitHub.
3.  Click **New Project** → **Deploy from GitHub repo**.
4.  Select your repository.
5.  Railway usually auto-detects Node.js.
6.  Go to **Settings** → **Root Directory** and set it to: `/Firmware/Dashboard/Src/Dashboard`.
7.  It will redeploy automatically.

---

## 🛠️ Testing Your Deployment

1.  Open your new URL (e.g., `https://...onrender.com`).
2.  Check the "Remote (IoT)" status dot.
3.  **Note**: Since your embedded device (ESP-01) is not yet connected to this _specific_ cloud URL, you won't see live data yet.
    - _Next Step for Embedded_: You will need to update the embedded code to connect to this new URL instead of a local IP.

## 📱 Mobile Access

Once deployed, just send the URL to your phone via WhatsApp/Telegram and open it in Chrome/Safari. You can even "Add to Home Screen" to make it look like an app!
