#!/usr/bin/env bash
# testes do TP1 — uso: ./tests.sh
set -u
cd "$(dirname "$0")" || exit 1
DIR=$(pwd)
make -s all || exit 1

TMP=$(mktemp -d) && [ -n "$TMP" ] || exit 1
trap 'rm -rf "$TMP"' EXIT
falhas=0

t_q1_100_caracteres() { [ "$("$DIR/Q1" | wc -c)" -eq 100 ]; }
t_q1_blocos_de_10_seguidos() {
 local out
 out=$("$DIR/Q1")
 [ "$(printf %s "$out" | fold -w10 | grep -Ec '^([0-9])\1{9}$')" -eq 10 ] &&
  [ "$(printf %s "$out" | fold -w10 | sort -u | wc -l)" -eq 10 ]
}

t_q2_chaine_filhos_do_mesmo_pai() {
 local out
 out=$("$DIR/Q2_chaine" 3) || return 1
 [ "$(printf '%s\n' "$out" | wc -l)" -eq 4 ] || return 1
 printf '%s\n' "$out" | awk 'NR == 1 {sub(",", "", $2); p = $2} NR > 1 && $4 != p {bad = 1} END {exit bad}'
}
t_q2_arbre_cada_um_filho_do_anterior() {
 local out
 out=$("$DIR/Q2_arbre" 3)
 [ "$(printf '%s\n' "$out" | wc -l)" -eq 4 ] || return 1
 # pai listado tem de ser a linha anterior (órfão tem pai fora da lista)
 printf '%s\n' "$out" | awk '
  {sub(",", "", $2); ppid[NR] = $4; linha[$2] = NR}
  END {for (k = 2; k <= NR; k++) if ((ppid[k] in linha) && linha[ppid[k]] != k - 1) exit 1}'
}

t_q3_executa() { [ "$("$DIR/Q3" echo ola mundo)" = "ola mundo" ]; }
t_q3_comando_inexistente() { ! "$DIR/Q3" comando_que_nao_existe 2>/dev/null; }

t_q4_cinco_vezes() { [ "$("$DIR/Q4" echo x | wc -l)" -eq 5 ]; }
t_q4_para_no_erro() (
 d=$(mktemp -d -p "$TMP") && cd "$d" || exit 1
 err=$("$DIR/Q4" mkdir toto 2>&1 >/dev/null) && exit 1
 [[ $err == *"execução 2"* ]]
)

t_q5_so_sai_no_quinto_ctrlc() {
 local pid vigia i ret
 "$DIR/Q5" >/dev/null &
 pid=$!
 (sleep 5; kill -KILL "$pid") 2>/dev/null &
 vigia=$!
 sleep 0.3
 for i in 1 2 3 4; do kill -INT "$pid"; sleep 0.1; done
 kill -0 "$pid" 2>/dev/null || { kill "$vigia"; return 1; }
 kill -INT "$pid"
 wait "$pid"
 ret=$?
 kill "$vigia" 2>/dev/null
 [ "$ret" -eq 0 ]
}

t_q6_mais_velho_morto_por_sigusr2() {
 local out="$TMP/q6.out" i
 for i in $(seq 10); do
  timeout 5 "$DIR/Q6" >"$out" || return 1
  grep -q 'terminado por SIGUSR2' "$out" && grep -q 'terminou$' "$out" || return 1
 done
}

t_q7_primeiro_pai_espera_o_ultimo() {
 local out="$TMP/q7.out" i
 for i in $(seq 10); do
  timeout 5 "$DIR/Q7" 3 >"$out" || return 1
  # logo após o retorno, todas as linhas já estão no arquivo
  [ "$(wc -l <"$out")" -eq 5 ] && tail -1 "$out" | grep -q '^pai inicial' || return 1
 done
}
t_q7_sem_corrida_com_n_1() {
 local i
 for i in $(seq 20); do timeout 5 "$DIR/Q7" 1 >/dev/null || return 1; done
}

t_q8_absent_present() (
 d=$(mktemp -d -p "$TMP") && cd "$d" || exit 1
 [ "$("$DIR/Q8" test -e toto alors echo present sinon echo absent)" = absent ] || exit 1
 mkdir toto
 [ "$("$DIR/Q8" test -e toto alors echo present sinon echo absent)" = present ]
)
t_q8_sem_sinon() { [ -z "$("$DIR/Q8" false alors echo sim)" ]; }
t_q8_sintaxe_invalida() { "$DIR/Q8" test -e x 2>/dev/null; [ $? -eq 2 ]; }

for t in $(declare -F | awk '{print $3}' | grep '^t_'); do
 if "$t"; then printf 'ok    %s\n' "$t"; else printf 'FALHA %s\n' "$t"; falhas=$((falhas + 1)); fi
done

echo
if [ "$falhas" -eq 0 ]; then
 echo "todos os testes passaram"
else
 echo "$falhas teste(s) falharam"
 exit 1
fi
