# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.


inherit image_types_phosphor

make_tar_of_images() {
        type=$1
        shift
        extra_files="$@"

        # Create the tar archive
        tar -h -cvf ${IMGDEPLOYDIR}/${IMAGE_NAME}.$type.tar \
        image-rofs image-rwfs $extra_files

        cd ${IMGDEPLOYDIR}
        ln -sf ${IMAGE_NAME}.$type.tar ${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.$type.tar
}

do_generate_ext4_tar() {
        # Generate the compressed ext4 rootfs
	xz -f -k -c ${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.${FLASH_EXT4_BASETYPE} > ${IMAGE_LINK_NAME}.${FLASH_EXT4_BASETYPE}.xz

        ln -sf ${IMAGE_LINK_NAME}.${FLASH_EXT4_BASETYPE}.xz image-rofs
        ln -sf ${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.rwfs.${FLASH_EXT4_OVERLAY_BASETYPE} image-rwfs
        ln -sf ${S}/MANIFEST MANIFEST
        ln -sf ${S}/publickey publickey

	if [ -e "${SYSMGR_BIN_PATH}/tcu-flash.bin" ]
	then
		ln -sf ${SYSMGR_BIN_PATH}/tcu-flash.bin tcu-flash.bin
		make_signatures  image-rofs image-rwfs MANIFEST publickey tcu-flash.bin
		make_tar_of_images ext4.mmc MANIFEST publickey ${signature_files} tcu-flash.bin
	else
		make_signatures  image-rofs image-rwfs MANIFEST publickey
		make_tar_of_images ext4.mmc MANIFEST publickey ${signature_files}
	fi
}

do_generate_ext4_tar[dirs] = " ${S}/ext4"
do_generate_ext4_tar[depends] += " \
        zstd-native:do_populate_sysroot \
        ${PN}:do_image_${FLASH_EXT4_BASETYPE} \
        openssl-native:do_populate_sysroot \
        ${SIGNING_KEY_DEPENDS} \
        ${PN}:do_copy_signing_pubkey \
        phosphor-hostfw-image:do_deploy \
        "
 
