# ElasticBSP
Dataset generators and source code (ElasticBSP is implemented in `C++11`)

We implement the dataset generators of the paper "[__ZipLine: an optimized algorithm for the elastic bulk synchronous parallel model__](https://link.springer.com/epdf/10.1007/s10994-021-06064-w?sharing_token=SL53OdVFrRUyIz8qjs80RPe4RwlQNchNByi7wbcMAY4ao99V3o1RGUuZsegmpbZzbGG7mCBCJwD1rQzYJwr5nN4k8N7gKw-8a-hnyHswBN1L7R6OOD1rSQd6dbdlb0ySGOtiwHCYV_UWeUfF6CCDvTPEW1_o_uD57Ek8SHPfa7Q%3D)"
, which simulate the real distributed running environment where a parallel computing service is running on mutiple workers upon execution.

This implenetation is explained in details in Section 6.1 _"ZipLine performance"_ of the paper. 
It generates the dataset mentioned in Table 2, "_Synthetic datasets with varying number of n and R_".

NOTE: Due to NDA, ZipLine source code is not available at the moment.

# Paper

[***ZipLine: an optimized algorithm for the elastic bulk synchronous parallel model***](https://link.springer.com/epdf/10.1007/s10994-021-06064-w?sharing_token=SL53OdVFrRUyIz8qjs80RPe4RwlQNchNByi7wbcMAY4ao99V3o1RGUuZsegmpbZzbGG7mCBCJwD1rQzYJwr5nN4k8N7gKw-8a-hnyHswBN1L7R6OOD1rSQd6dbdlb0ySGOtiwHCYV_UWeUfF6CCDvTPEW1_o_uD57Ek8SHPfa7Q%3D)

# Talk in IEEE DSAA 2021

[<img src="DSAA21Talk.png" width="650">](https://youtu.be/NQsjbqaNimk)


# Use case

Prerequisite: `gcc 4.8+`

The data generators simulate the `pull and push requests` from mutiple workers and ouput the timestamps of push request of workers in a `n x R` matrix format 
(**n**: number of workers and **R**: the range of future iterations or the **R** future iterations of **n** workers).
For example, there are 10 workers and we want to estimate their next 20 future iterations at some time point, the generators will output a dataset in `10 x 20` matrix format.
The dataset has 10 entries and each entry has 20 timestampes of workers' `push requsts`.

To use these data generators to produce the datasets that are mentioned in Section 6.1 of the [paper](https://link.springer.com/epdf/10.1007/s10994-021-06064-w?sharing_token=SL53OdVFrRUyIz8qjs80RPe4RwlQNchNByi7wbcMAY4ao99V3o1RGUuZsegmpbZzbGG7mCBCJwD1rQzYJwr5nN4k8N7gKw-8a-hnyHswBN1L7R6OOD1rSQd6dbdlb0ySGOtiwHCYV_UWeUfF6CCDvTPEW1_o_uD57Ek8SHPfa7Q%3D)
, please follow the steps below:

Under the downloaded/cloned directory, i.e., `ElasticBSP/`

1. Compile the two generators, `workers_iterations_gen.exe` and `future_iterations_gen.exe`.

   ```
   g++ -std=c++11 -o workers_iterations_gen.exe workers_iteration_intervals_gen.cc
   
   g++ -std=c++11 -o future_iterations_gen.exe future_iteration_intervals_gen.cc
   ```

2. Use `workers_iterations_gen.exe` to ouput a matrix of timestamps of workers which simulates **n** workers initiating a distributed services near a same starting time point 
and running up to a user-speficied time point (timestamp).

   ```
   workers_iterations_gen.exe [number of workers] [number of timestamps] [output filename]
   ```
   
   For example, we want to generate 10 initial timestamps (consider timestamps from the starting of the services till 10th `push request` inclusively) for 20 workers, 
   and save them (a matrix) to an output file `init_data_gen_matrix_worker20_t10.txt`.
   
   ```
   workers_iterations_gen.exe 20 10 init_data_gen_matrix_worker20_t10.txt
   ```

3. Use `future_iterations_gen.exe` to output a matrix of future timestamps of workers given the init timestamps output from **step 2**, and the number of future iterations
   of workers (it can detect the number of the workers by reading the init output file). The final output is a `n x R` matrix where **n** is the number of workers 
   and **R** is the range of the future iterations (or **R** future iterations) for every worker.
   The output file name is auto-generated based on the input **n** and **R**.
   
   ```
   future_iterations_gen.exe [init timestamps filename from step 2] [the range R]
   ```
   
   Following the example from **step 2**, we want to generate 30 future iterations for all 20 workers.
   
   ```
   future_iterations_gen.exe init_data_gen_matrix_worker20_t10.txt 30
   ```
   
   It will output file `data_gen_future_iteration_workers_n20_R30.txt` which contains 20 entries and each entry has 30 estimated future timestamps (`20 x 30` matrix).
   
# Reference

```
@article{10.1007/s10994-021-06064-w, 
year = {2021}, 
title = {{ZipLine: an optimized algorithm for the elastic bulk synchronous parallel model}}, 
author = {Zhao, Xing and Papagelis, Manos and An, Aijun and Chen, Bao Xin and Liu, Junfeng and Hu, Yonggang}, 
journal = {Machine Learning}, 
issn = {0885-6125}, 
doi = {10.1007/s10994-021-06064-w}, 
pages = {1--37}
}

@inproceedings{zhao2019elastic,
  title={Elastic Bulk Synchronous Parallel Model for Distributed Deep Learning},
  author={Zhao, Xing and Papagelis, Manos and An, Aijun and Chen, Bao Xin and Liu, Junfeng and Hu, Yonggang},
  booktitle={19th IEEE International Conference on Data Mining (ICDM)},
  pages={1504-1509},
  year={2019},
}
``` 
