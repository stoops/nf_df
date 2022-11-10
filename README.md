# nf_df

# req

v="5.X.Y-Z-amd64" ; apt install build-essential linux-image-$v linux-headers-$v

# exe

make ; insmod nf_df.ko

# doc

clears the don't fragment bit in the ipv4 header - linux kernel nftables packet rewrite - similar to the bsd pf scrub in all no-df
