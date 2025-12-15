#!/bin/bash

##############################################################################
# Smart Energy Dashboard - Quick Start Script
# This script automates the setup and launch of the dashboard
##############################################################################

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Banner
echo -e "${BLUE}"
echo "═══════════════════════════════════════════════════════════"
echo "⚡ Smart Energy Management System - Dashboard Setup"
echo "═══════════════════════════════════════════════════════════"
echo -e "${NC}"

# Function to print colored messages
print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

# Check if Node.js is installed
print_info "Checking Node.js installation..."
if ! command -v node &> /dev/null; then
    print_error "Node.js is not installed!"
    echo ""
    echo "Please install Node.js from: https://nodejs.org/"
    echo "Recommended version: 16.x or higher"
    exit 1
fi

NODE_VERSION=$(node -v)
print_success "Node.js found: $NODE_VERSION"

# Check if npm is installed
print_info "Checking npm installation..."
if ! command -v npm &> /dev/null; then
    print_error "npm is not installed!"
    exit 1
fi

NPM_VERSION=$(npm -v)
print_success "npm found: $NPM_VERSION"

# Check if node_modules exists
# Check if node_modules and express exist
if [ ! -d "node_modules" ] || [ ! -d "node_modules/express" ]; then
    print_warning "Dependencies not installed or incomplete"
    print_info "Installing dependencies..."
    npm install
    
    if [ $? -eq 0 ]; then
        print_success "Dependencies installed successfully"
    else
        print_error "Failed to install dependencies"
        exit 1
    fi
else
    print_success "Dependencies already installed"
fi


echo ""
print_info "Starting dashboard server..."
echo ""

# Check if port 3000 is already in use
if lsof -Pi :3000 -sTCP:LISTEN -t >/dev/null 2>&1 ; then
    print_warning "Port 3000 is already in use!"
    print_info "Attempting to stop existing server..."
    
    # Kill existing process on port 3000
    kill -9 $(lsof -ti:3000) 2>/dev/null || true
    
    sleep 1
    print_success "Port cleared successfully"
    echo ""
fi

# Function to open browser
open_browser() {
    sleep 2  # Wait for server to start
    URL="http://localhost:3000"
    
    print_info "Opening dashboard in browser..."
    
    if command -v xdg-open > /dev/null; then
        xdg-open "$URL" 2>/dev/null &
    elif command -v sensible-browser > /dev/null; then
        sensible-browser "$URL" 2>/dev/null &
    elif command -v gnome-open > /dev/null; then
        gnome-open "$URL" 2>/dev/null &
    else
        print_warning "Could not auto-open browser"
        echo ""
        echo "Please manually open your browser and navigate to:"
        echo "  ${GREEN}${URL}${NC}"
    fi
}

# Display startup message
echo -e "${GREEN}"
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                 🚀 GESTELL ENERGY DASHBOARD 🚀                 ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "📍 Dashboard will be available at:"
echo "   ${BLUE}http://localhost:3000${GREEN}"
echo ""
echo "🎮 Quick Start:"
echo "   1. Dashboard will open automatically in your browser"
echo "   2. Click 'Demo Mode' to see simulated data"
echo "   3. Click 'Connect Device' for real hardware connection"
echo ""
echo "💡 Press Ctrl+C to stop the server"
echo "╚════════════════════════════════════════════════════════════════╝"
echo -e "${NC}"
echo ""

# Open browser in background
open_browser &

# Start the server
npm start

