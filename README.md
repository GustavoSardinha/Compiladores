# Compiladores

Repositório de estudos e projetos da disciplina de Compiladores.

---

## 📥 Clonar o repositório

Para clonar este repositório em sua máquina local:

```bash
git clone https://github.com/GustavoSardinha/Compiladores.git
cd Compiladores
```

Se você já tiver clonado e quiser trazer as últimas alterações do GitHub:
```bash
git pull origin main
```
Como compilar:
```bash
cd Compiladores
gcc main.c AnalisadorLexico/src/analisador.c -I AnalisadorLexico/include -o analisador
```
Execução no Windows:
```bash
analisador
```
Execução no Linux:
```bash
./analisador
```
