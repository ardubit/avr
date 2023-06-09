# Title: AVR toolchain installer macOS
# Author: Aleksey M.
# Latest version https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers
# http://ww1.microchip.com/downloads/en/DeviceDoc/avr8-gnu-toolchain-osx-3.6.2.503-darwin.any.x86_64.tar.gz
# http://packs.download.atmel.com/Atmel.ATmega_DFP.1.3.300.atpack

TOOLCHAIN="toolchain/avr8-gnu-toolchain-osx-3.7.0.518-darwin.any.x86_64.tar.gz"
ATPACK="atpack/Atmel.ATmega_DFP.1.3.300.atpack"

echo "AVR toolchain installer macOS"
echo "Rev.1.0 - 09.06.2023"

echo "Download and place files:
script itself   -> avr_toolchain_install.sh
into folder     -> atpack/Atmel DFP here
into folder     -> toolchain/Atmel Toolchain here
"

mkdir $HOME/Library/avr-atpack
sudo unzip $ATPACK -d $HOME/Library/avr-atpack/

mkdir $HOME/Library/avr-toolchain
tar xzf $TOOLCHAIN --directory .
sudo mv avr8-gnu-toolchain-darwin_x86_64 $HOME/Library/avr-toolchain/

echo "Bash shell path"
echo "---------------"
sudo sh -c 'echo "export PATH=\"$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:\$PATH\"" >> $HOME/.bash_profile'
# sudo sh -c 'echo "export PATH="$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:\$PATH"" >> $HOME/.bash_profile'
echo "Check export PATH here:"
sudo nano .bash_profile

echo "Zsh shell path"
echo "---------------"
sudo echo 'export PATH="$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:$PATH"' >> ~/.zshrc
echo "Check export PATH here:"
sudo nano .zshrc

echo $PATH

echo "DONE!"
echo "Restart your terminal or run by command: source ~/.zshrc"
echo "Or reboot your machine and test it using the terminal command avr-gcc -v OR avr-gcc --version"


