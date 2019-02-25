# Binarized CP Decomposition (B-CP)

This repository contains the code of the main experiments presented in the papers:

[Binarized Knowledge Graph Embeddings]
(http://arxiv.org/abs/1902.02970),
Koki Kishimoto, Katsuhiko Hayashi, Genki Akai, Masashi Shimbo, and Kazunori Komatani, ECIR 2019

## Using the Code

### Quickstart

To run the experiments, unpack the datasets first:
```
unzip datasets/FB15k-237.zip -d datasets/
```
Run with
```
sh train_and_test.sh
```
### Exapmle: B-CP on WN18RR
1. Unpack the datasets and make directory to save model
```
unzip datasets/WN18RR.zip -d datasets/
mkdir model
```
2. Compile the code for train and evaluate
```
make all
```
3. Add triples which have "inverse" relation [Kazemi+. 2018]
```
./add_rev.out -file_name datasets/WN18RR/train.txt -rev_filename datasets/WN18RR/train_rev.txt
-file_name                   Input datasets
-rev_file_name               Path to write output datasets which have triple included "inverse" relation
```
4. Train B-CP model
```
./quantizedcp.out -train datasets/WN18RR/train_rev.txt -dimension 200 -iteration 400 -rate 0.025 -model_dir model
-train                       Input datasets
-dimension                   Vector dimension
-iteration                   Number of epochs to train
-rate                        training rate
-model_dir                   Path to write output entity or relation vectors
```
5. Evaluate B-CP model
```
./testcp.out -train ./datasets/WN18RR/train_rev.txt -test ./datasets/WN18RR/test.txt  -valid ./datasets/WN18RR/valid.txt -subject_model "model/400_subject.txt" -object_model "model/400_object.txt" -relation_model "model/400_relation.txt"
-train                       Input train datasets
-test                        Input test datasets
-valid                       Input valid datasets               
-subject_model               Path to write output subject vectors
-object_model                Path to write output object vectors
-relation_model              Path to write output relation vectors
```

## Reference
* Seyed Mehran Kazemi and David Poole. Simple embed- ding for link prediction in knowledge graphs. In Proc. of the 32nd Annual Conference on Neural Information Processing Systems (NeurIPS), pages 4289–4300, 2018.
