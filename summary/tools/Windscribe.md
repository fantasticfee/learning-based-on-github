# ubuntu配置

*2. Add the Windscribe signing key to apt

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-key FDC247B7
*3. Add the repository to your sources.list

echo 'deb https://repo.windscribe.com/ubuntu zesty main' | sudo tee /etc/apt/sources.list.d/windscribe-repo.list
*4. Run apt-get update

sudo apt-get update
*5. Install windscribe-cli

sudo apt-get install windscribe-cli

*6. Login to Windscribe

windscribe login
*7. Connect to Windscribe

windscribe connect
