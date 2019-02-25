#!/bin/sh

model_dir=./model

make all

./add_rev.out -file_name ./datasets/FB15k-237/train.txt -rev_filename ./datasets/FB15k-237/train_rev.txt

mkdir -p $model_dir
./quantizedcp.out -train ./datasets/FB15k-237/train_rev.txt \
	-dimension 200 \
	-iteration 400 \
	-rate 0.025 \
	-model_dir $model_dir

./testcp.out -train ./datasets/FB15k-237/train_rev.txt \
	-test ./datasets/FB15k-237/test.txt \
	-valid ./datasets/FB15k-237/valid.txt \
	-subject_model "${model_dir}/400_subject.txt" \
	-object_model "${model_dir}/400_object.txt" \
	-relation_model "${model_dir}/400_relation.txt"
