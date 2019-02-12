#!/bin/sh

model_dir=./model

make all

./add_rev.out -file_name ./wordnet-mlj12/wordnet-mlj12-train.txt -rev_filename ./wordnet-mlj12/a.txt

mkdir -p $model_dir
./quantizedcp.out -train ./wordnet-mlj12/a.txt \
	-dimension 200 \
	-iteration 100 \
	-rate 0.025 \
	-lambda 0.0001 \
	-model_dir $model_dir

./testcp.out -train ./wordnet-mlj12/a.txt \
	-test ./wordnet-mlj12/wordnet-mlj12-test.txt \
	-valid ./wordnet-mlj12/wordnet-mlj12-valid.txt \
	-subject_model "${model_dir}/100_subject.txt" \
	-object_model "${model_dir}/100_object.txt" \
	-relation_model "${model_dir}/100_relation.txt"
