# DCGAN
This is "Deep Convolutional Generative Adversarial Networks".<br>
Original paper: A. Radford, L. Metz, and S. Chintala. Unsupervised representation learning with deep convolutional generative adversarial networks. In International Conference on Learning Representations, 2016. [link](https://arxiv.org/abs/1511.06434)

## Usage

### 1. Build
Please build the source file according to the procedure.
~~~
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
$ cd ..
~~~

### 2. Dataset Setting

#### Recommendation
- Large-scale CelebFaces Attributes (CelebA) Dataset<br>
This is a large-scale face attributes dataset with more than 200K celebrity images, each with 40 attribute annotations.<br>
Link: [official](http://mmlab.ie.cuhk.edu.hk/projects/CelebA.html)

#### Setting

Please create a link for the dataset.<br>
You should substitute the path of training data for "<training_path>".<br>
The following is an example for "celebA".
~~~
$ cd datasets
$ mkdir celebA
$ cd celebA
$ ln -s <training_path> ./train
$ cd ../..
~~~

### 3. Training

#### Setting
Please set the shell for executable file.
~~~
$ vi scripts/train.sh
~~~
The following is an exmaple of the training phase.<br>
If you want to view specific examples of command line arguments, please view "src/main.cpp" or add "--help" to the argument.
~~~
#!/bin/bash

DATA='celebA'

./DCGAN \
    --train true \
    --epochs 300 \
    --dataset ${DATA} \
    --size 256 \
    --loss "vanilla" \
    --batch_size 16 \
    --gpu_id 0 \
    --nc 3
~~~

#### Run
Please execute the following to start the program.
~~~
$ sh scripts/train.sh
~~~

### 4. Image Generation

#### Setting
Please set the shell for executable file.
~~~
$ vi scripts/test.sh
~~~
The following is an exmaple of the test phase.<br>
If you want to view specific examples of command line arguments, please view "src/main.cpp" or add "--help" to the argument.
~~~
#!/bin/bash

DATA='celebA'

./DCGAN \
    --test true \
    --dataset ${DATA} \
    --size 256 \
    --gpu_id 0 \
    --nc 3
~~~
If you want to generate image, the above settings will work.

#### Run
Please execute the following to start the program.
~~~
$ sh scripts/test.sh
~~~

