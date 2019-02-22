#!/bin/sh

unzip ./FB15k-237.zip

model_dir=./model

make all

./add_rev.out -file_name ./FB15k-237/train.txt -rev_filename ./FB15k-237/a.txt

mkdir -p $model_dir
./quantizedcp.out -train ./FB15k-237/a.txt \
	-dimension 200 \
	-iteration 100 \
	-rate 0.025 \
	-lambda 0.0001 \
	-model_dir $model_dir

./testcp.out -train ./FB15k-237/a.txt \
	-test ./FB15k-237/test.txt \
	-valid ./FB15k-237/valid.txt \
	-subject_model "${model_dir}/100_subject.txt" \
	-object_model "${model_dir}/100_object.txt" \
	-relation_model "${model_dir}/100_relation.txt"
