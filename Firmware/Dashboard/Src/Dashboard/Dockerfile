# Use official Node.js image
FROM node:18-alpine

# Set working directory
WORKDIR /app

# Copy package files
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy project files
COPY . .

# Expose port (Koyeb/Render usually use 8000 or 3000)
ENV PORT=3000
EXPOSE 3000

# Start command
CMD ["node", "server.js"]
