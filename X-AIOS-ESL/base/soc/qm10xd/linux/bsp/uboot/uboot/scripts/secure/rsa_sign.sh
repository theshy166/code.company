#!/bin/bash

IMAGE_FILE=$1
KEY_FILE=$2
IMAGE_HEADER_LEN=$3
padding=$4

# function: usage
function usage() {
    echo -e "
Usage: ./rsa_sign.sh <u-boot.bin> <key> <header-len> <padding>
  Where,
      u-boot.bin: The bootloader that is to be signed.
             key: The RSA private key file. Ex: rsa_priv.pem
      header-len: The U-Boot file header length.
      padding: The RSA padding mode (pss or pkcs1).

  Command Example:

     ./rsa_sign.sh u-boot.bin rsa_priv.pem header-len pss
    "
}

function debug_print() {
    local message="$1"
    if [ ${debug} -ne 0 ]; then
        echo "${message}"
    fi
}

function sign_img() {
    local wb="$1"
    local sec_offset="$2"
    local key="$3"
    local klen=256
    local rsa_e_file=rsa_e
    local size_file=size

    # Define new offsets
    local sig_offset=0x300  # Signature offset
    local mod_offset=0x200  # Public key modulus (N) offset
    local e_offset=0x1f0    # Public key exponent (e) offset

    debug_print "Extract the part of the binary that needs to be signed"
    dd bs=1 skip=1024 if="${wb}" of="${wb}.tosig.tmp" > /dev/null 2>&1
    filesize=$(wc -c < "${wb}.tosig.tmp") > /dev/null
    actual=$(( (filesize + 15) / 16 * 16 )) > /dev/null

    debug_print "filesize = ${filesize} actual = ${actual}"
    debug_print "sig_offset = ${sig_offset} mod_offset = ${mod_offset} e_offset = ${e_offset}"

    objcopy -I binary -O binary --gap-fill=0x00 --pad-to="${actual}" "${wb}.tosig.tmp" "${wb}.tosig"

    # Calculate RSA signature
    if [ "${padding}" == 'pss' ]; then
        openssl dgst -sha256 -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1 -sign "${key}" -out "${wb}.rsa.sig" "${wb}.tosig"
    else
        openssl dgst -sha256 -sigopt rsa_padding_mode:pkcs1 -sign "${key}" -out "${wb}.rsa.sig" "${wb}.tosig"
    fi

    # Inject RSA signature into the image
    dd conv=notrunc bs=1 seek=$((sig_offset)) count=256 if="${wb}.rsa.sig" of="${wb}" > /dev/null 2>&1

    # Extract and reverse the public key modulus (N)
    openssl rsa -in "${key}" -noout -modulus -out "${key}.mod"
    cut -d= -f2 < "${key}.mod" > "${key}.mod.tmp"
    xxd -r -p -l 256 "${key}.mod.tmp" "${key}.mod.tmp2"
    objcopy -I binary --reverse-bytes=256 "${key}.mod.tmp2" "${key}.mod.rev"

    # Inject public key modulus into the image
    dd conv=notrunc bs=1 seek=$((mod_offset)) count=256 if="${key}.mod.rev" of="${wb}" > /dev/null 2>&1

    # Inject public key exponent (e) into the image
    echo "01000100000000000000000000000000" > "${rsa_e_file}.tmp"
    xxd -r -p -l 16 "${rsa_e_file}.tmp" "${rsa_e_file}.bin.tmp"
    dd conv=notrunc bs=1 seek=$((e_offset)) count=16 if="${rsa_e_file}.bin.tmp" of="${wb}" > /dev/null 2>&1

    # Calculate the hash of the public key (E and N) from 0x1f0 to 0x300
    dd skip=$((e_offset)) bs=1 count=$((0x300 - 0x1f0)) if="${wb}" of="${wb}.pubkey.tmp" > /dev/null 2>&1
    openssl dgst -sha256 "${wb}.pubkey.tmp"

    # Replace filesize with actual size in the image header
    printf "%08x" "${actual}" > "${size_file}.tmp"
    xxd -r -p -l 4 "${size_file}.tmp" "${size_file}.bin.tmp"
    objcopy -I binary --reverse-bytes=4 "${size_file}.bin.tmp" "${size_file}.bin.rev"
    dd conv=notrunc bs=1 seek=8 count=4 if="${size_file}.bin.rev" of="${wb}" > /dev/null 2>&1

    dd skip="${mod_offset}" bs=1 count=260 if="${wb}" of="${wb}.pub.bin" > /dev/null 2>&1
    #openssl dgst -sha256 "${wb}.pub.bin"

    debug_print "Image signed to file ${wb}"
}


# Main
debug=0

# Check input files
if [ $# -lt 4 ]; then
    usage
    exit 1
fi

IMAGE_FILE=$(readlink -f "${IMAGE_FILE}")
KEY_FILE=$(readlink -f "${KEY_FILE}")

sign_img "${IMAGE_FILE}" "${IMAGE_HEADER_LEN}" "${KEY_FILE}" "${padding}"

# Clean up all tmp files
if [ ${debug} -eq 0 ]; then
    rm -f *.sig *.tosig *.mod *.rev *.tmp *.tmp2
fi
