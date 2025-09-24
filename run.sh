#!/usr/bin/env bash

exe="./NTUlegalize"
benchmarks_dir="benchmarks"
output_dir="output"
plot_dir="plot"
plot_tmp_dir="plot_tmp"

all_benchmarks=()
for f in "$benchmarks_dir"/*; do
  all_benchmarks+=("$(basename "$f")")
done

benchmarks=()
echo "Which benchmark(s) would you like to run? (Enter number(s) or 'all')"
select dummy in "${all_benchmarks[@]}"; do
  IFS=', ' read -r -a choices <<< "$REPLY"

  if [[ "${choices[0]}" == "all" ]]; then
    benchmarks=("${all_benchmarks[@]}")
  else
    for choice in "${choices[@]}"; do
      benchmarks+=("${all_benchmarks[choice - 1]}")
    done
  fi

  break
done

if [[ ! -d "$output_dir" ]]; then
  mkdir "$output_dir"
fi

if [[ ! -d "$plot_dir" ]]; then
  mkdir "$plot_dir"
fi

if [[ ! -d "$plot_tmp_dir" ]]; then
  mkdir "$plot_tmp_dir"
fi

is_site_height2="false"
read -r -p "Is Site Height = 2.0? [y/N] "
if [[ "$REPLY" =~ ^[Yy]$ ]]; then
  is_site_height2="true"
fi

is_pgp="false"
read -r -p "Plot Before Detailed placement? [y/N] "
if [[ "$REPLY" =~ ^[Yy]$ ]]; then
  is_pgp="true"
fi

is_plg="false"
read -r -p "Plot After Detailed placement? [y/N] "
if [[ "$REPLY" =~ ^[Yy]$ ]]; then
  is_plg="true"
fi

# ""$benchmarks_dir"/"$benchmark"/placed_lg_pre_3CH.def" \
for benchmark in "${benchmarks[@]}"; do
  
  if [[ "$is_site_height2" == "true" ]]; then
    command=""$exe" \
    ""$benchmarks_dir"/"$benchmark"/tech_2CH.lef" \
    ""$benchmarks_dir"/"$benchmark"/cells_2CH.lef" \
    ""$benchmarks_dir"/"$benchmark"/placed_lg_2CH.def" \
    ""$benchmarks_dir"/"$benchmark"/placement.constraints" \
    ""$output_dir"/"$benchmark"_lg_pre.def" \
    --cpu 8"
  else
    command=""$exe" \
    ""$benchmarks_dir"/"$benchmark"/tech_3CH_old.lef" \
    ""$benchmarks_dir"/"$benchmark"/cells_3CH_old.lef" \
    ""$benchmarks_dir"/"$benchmark"/placed_lg_3CH.def" \
    ""$benchmarks_dir"/"$benchmark"/placement.constraints" \
    ""$output_dir"/"$benchmark"_lg.def" \
    --cpu 8"
  fi

  if [[ "$is_pgp" == "true" ]]; then
    command=""$command" --pgp "$plot_dir"/"$benchmark"_bd.plt"
  fi

  if [[ "$is_plg" == "true" ]]; then
    command=""$command" --plg "$plot_dir"/"$benchmark"_ad.plt"
  fi

  echo "Run "$benchmark"..."
  # echo "$command"
  echo ""
  result="$(time $command 2>&1 | tee /dev/tty)"
  echo ""
done
