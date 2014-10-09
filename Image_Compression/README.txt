README
Homework 4
by Gabrielle Cella and Ann Bellinger (pair programming project)
10/15/13


This program performs two functions:
        ~Compressesion
		~Usage:
			-> ./40image -c ostridge.ppm > compressed_image.txt
                ~Trims edges of image to even number of pixel height/width
		 if necessary
                ~Compresses image to 32-bit words
                ~Reads out compressed image to standard out with appropriate
                 header in big-endian order
        ~Decompression
		~Usage:
			-> ./40image -d compressed_image.txt > decompressed.ppm
                ~Reads in 32-bit words in big-endian order
                ~Decompresses 32-bit words to original image
		~Produces appropriate ppm file
                ~Can decompress other classmates' compressed images
	~To compile:
		-> ./compile

Concerns:
        ~There is slight pixilization of the decompressed image but this is most
         likely due to the averaging, quantinization, and dequantinization of 
         numbers. We believe this is on-par with jpeg compression.

Architecture:
                The abstract data types we are dealing with here are 
                UArray2_T's (Abstract Data Type for a 2D array), UArray2b_T's 
		(Abstract Data Type for a 2D blocked array), and Pnm_ppm 
		structs that are typedeffed in our processes.h file so as not to
		confuse the different intermediary components. 

        ~Image Compression
                We start by reading in the file and storing the pixels in a 
                UArray2_T of Pnm_rgb structs that is held in a Pnm_ppm. We
                then convert the rgb values to Y, PB, and PR values for each 
                pixel and store these new cv_info structs in a UArray2b_T 
                inside a Pnm_cv. We do this by mapping_row_major through the 
                Pnm_ppm, which holds our original image, with the Pnm_cv to be 
                filled as the closure. 
                Next we create a Pnm_components, which holds a UArray2_T of 
                Components structs that is half the width and height of the 
                UArray2b_T stored in the Pnm_cv struct. We map_row_major through
                the UArray2_T in Pnm_components and calculate the a, b, c, d, 
                PRchroma, and PBchroma based on the data in the corresponding 
                blocks in the Pnm_cv's struct, which is passed as closure.
                Lastly, we pack the Components values into 32-bit words, which 
                are stored in a new UArray2_T. These values are then mapped 
                through in map_row_major and printed in big-endian order. 
        ~Image Decompression
                We start by reading in the file and storing the 32-bit words in
                a UArray2_T. We then map_row_major through the UArray2_T of 
                32-bit words and retrieve the a, b, c, d, PBchroma, and PRchroma
                values from each word, storing these values in a Components
                struct inside of a UArray2_T. This array is held in a 
                Pnm_components struct. 
                Next we create a Pnm_cv struct, which has a UArray2b_T that 
                holds Pnm_cv structs. We map_row_major through the 
                Pnm_components' UArray2_T, using the Pnm_cv struct as closure. 
                We fill the Pnm_cv's cv_info elements with the respective Y, PB,
                and PR values from inverse discrete cosine transformation. 
                Lastly we map_block_major through the UArray2b_T in this Pnm_cv,
                filling the final UArray2_T in a Pnm_ppm, which is passed as the
                closure. The members of each cv_info struct are used to 
                calculate the R, G, and B values. We use ultimately read out the
                Pnm_rgb's to standard out usign Pnm_ppmwrite(). 

