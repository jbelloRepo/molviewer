<h1>
  <img align="center" height="30" src="icon.png" style="vertical-align: text-bottom;"> Molviewer App
</h1>

> MolViewer provides an immersive and interactive platform to explore and visualize the intricate structures of chemical molecules. 


## Table of Contents
## Table of Contents
- [Description](#description)
- [Installation](#installation)
  1. [Clone the Repository](#clone-the-repository)
  2. [Docker Installation](#docker-installation)
  3. [Containerize the Application](#containerize-the-application)
  4. [Compile the C Program](#compile-the-c-program)
  5. [Start the App](#start-the-app)
  6. [Access the App](#access-the-app)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Description
Molviewer is a powerful application for visualizing molecular structures. This README provides an overview of the Molviewer app, including installation instructions, usage guidelines, and contribution details.

## Installation
To install and run the Molview app, follow these steps:

1. **Clone the Repository**: 
- Clone the Molview repository to your local machine using the following command:
    ```
     git clone https://github.com/jbelloRepo/molviewer.git
    ```
2. **Docker Installation**: 
- This application requires several dependencies to be installed in order to run correctly. To save you the trouble of manually installing all necessary dependencies, the application has been packaged into a docker container. It contains everything needed to run the application.
- Install Docker by following the instructions specific to your operating system. You can find the installation guide [here](https://docs.docker.com/get-docker/).

3. **Containerize the Application**:
- Change directory to the app directory. Replace `/path/to/app` with the path to git repository directory.
    ```
     cd /path/to/app
    ```


### Docker Installation



docker run -p 8000:8000 -v $PWD/molviewerapp:/molviewerapp -it fullstack/molviewerapp   
docker build . -t fullstack/molviewerapp 