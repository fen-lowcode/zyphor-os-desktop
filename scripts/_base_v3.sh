apt install wget

wget https://github.com/zyphor-os/zyphor-os.github.io/raw/refs/heads/main/bethany-lts/bethany/pool/main/z/zyphor-archive-keyring.deb

wget https://github.com/zyphor-os/zyphor-os.github.io/raw/refs/heads/main/bethany-lts/bethany/pool/main/z/zyphor-repo-config.deb

apt install ./zyphor-archive-keyring.deb

apt install ./zyphor-repo-config.deb

# apps

apt install zyphor-cli

apt install zyphor-plymouth

apt install zyphor-fastfetch-config

apt install zyphor-face-icon

apt install zyphor-desktop-environment

apt install zyphor-bashrc-config

apt install zycamera-launcher

rm -rf ./zyphor-archive-keyring.deb

rm -rf ./zyphor-repo-config.deb

zy system upgrade

zy system clean