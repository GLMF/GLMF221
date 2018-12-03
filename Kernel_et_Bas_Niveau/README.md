# RPN : Extension de la syntaxe grâce à lex
par Martial Bornet [Consultant système, auteur du livre "Expressions Régulières, syntaxe et mise en oeuvre" chez ENI, créateur de la commande de colorisation "hl"]

---

Dans un article précédent (GLMF numéro 217), nous avons vu comment créer les bases d'un interpréteur de formules en notation polonaise inversée. Cet interpréteur minimal a été validé avec de petits programmes de tests élémentaires, à la syntaxe rigide. Dans le présent article, nous allons voir comment rendre la syntaxe de l'interpréteur plus souple grâce au générateur d'analyseurs lexicaux Lex, et comment ajouter des types nouveaux, comme des vecteurs, des matrices, et, ultérieurement, des listes, des textes, ou encore des réseaux de neurones.
