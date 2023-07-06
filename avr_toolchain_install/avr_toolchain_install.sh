#!/bin/bash
# Title: AVR toolchain installer macOS
# Author: Aleksey M.
# Run the script from terminal: ./avr_toolchain_install.sh

# Latest version https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers
# http://ww1.microchip.com/downloads/en/DeviceDoc/avr8-gnu-toolchain-osx-3.6.2.503-darwin.any.x86_64.tar.gz
# http://packs.download.atmel.com/Atmel.ATmega_DFP.1.3.300.atpack

TOOLCHAIN="toolchain/avr8-gnu-toolchain-osx-3.7.0.518-darwin.any.x86_64.tar.gz"
ATPACK="atpack/Atmel.ATmega_DFP.1.3.300.atpack"

echo "AVR toolchain installer macOS"
echo "=== Rev.1.0 - 06.07.2023 ==="

echo "Download and place files:"
echo "-> avr_toolchain_install.sh"
echo "-> atpack/[Atmel DFP here]"
echo "-> toolchain/[Atmel Toolchain here]"
echo ""

echo "Do you want to proceed? (y/n)"
read response

if [ "$response" = "n" ]; then
    echo "Aborting..."
    exit 1  # Terminate the script
else
    echo "Proceeding..."
fi

mkdir "$HOME/Library/avr-atpack"
unzip "$ATPACK" -d "$HOME/Library/avr-atpack/"

mkdir "$HOME/Library/avr-toolchain"
tar xzf "$TOOLCHAIN" --directory .

sudo mv "avr8-gnu-toolchain-darwin_x86_64" "$HOME/Library/avr-toolchain/"

echo "Bash shell path"
echo "---"
sudo sh -c 'echo "export PATH=\"$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:\$PATH\"" >> $HOME/.bash_profile'

# sudo sh -c 'echo "export PATH=\"$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:\$PATH\"" >> $HOME/.bash_profile'

echo "Verify export PATH:"
nano "$HOME/.bash_profile"

echo "Zsh shell path"
echo "---"
sudo sh -c 'echo "export PATH=\"$HOME/Library/avr-toolchain/avr8-gnu-toolchain-darwin_x86_64/bin:\$PATH\"" >> $HOME/.zshrc'

echo "Verify export PATH:"
nano "$HOME/.zshrc"

# Show current PATH
echo "$PATH"

echo "DONE!"
echo "Restart terminal or run the command: source ~/.zshrc"
echo "Reboot your machine and test it using the terminal command: avr-gcc -v OR avr-gcc --version"