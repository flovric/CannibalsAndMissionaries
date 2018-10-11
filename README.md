# CannibalsAndMissionaries

Solution for Cannibals and Missionaries problem in C programming language using message queues.

Full text of an assignment in Croatian:

Kanibali i misionari

"Na obali neke široke rijeke postoji jedan čamac koji prevozi kanibale i misionare na drugu stranu rijeke. Kapacitet čamca je 3 putnika i u čamcu mora biti najmanje 1 putnik da on može krenuti. U čamcu ne smije biti više kanibala od misionara, dok su sve ostale kombinacije putnika dozvoljene. Na početku glavni proces, koji ujedno predstavlja proces čamac, stvara N procesa kanibala i M procesa misionara (N>1 i M>1 se zadaju). Svi misionari, kanibali i čamac se na početku nalaze na istoj strani rijeke. Kanibali i misionari žele čamcem prijeći na drugu stranu rijeke. Nakon što prijeđu preko rijeke putnici odlaze dalje (nema ih više u sustavu, tj. proces kanibala ili misonara završi), a čamac se vraća na obalu s koje je krenuo. Čamac čeka na ukrcaj 4 sekunde i krene ako se ukrcao barem 1 putnik. Ako se nitko nije ukrcao nakon 4 sekunde proces čamac završi s radom. Proces čamac pri svakom prijelazu ispisuje koga prevozi (npr. "Prevezeni: misionar, misionar, kanibal"). Procesi međusobno komuniciraju uz pomoć reda poruka.

Ispravno sinkronizirati procese kanibale, misionare i čamac koristeći protokol s putujućom značkom.

Svi procesi ispisuju poruku koju šalju i poruku koju primaju."
