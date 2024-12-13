## A block

```
+ ---- +
| Icon |
+ ---- +
```

## A row of blocks

```
+ ---- + --------- + -------- +
| Menu | Our Chefs | About us |
+ ---- + --------- + -------- +
```

## A column of blocks

```
+ ------------- +
| Why Sex Toys  |
+ ------------- +
| catalogue     |
+ ------------- +
| My Collection |
+ ------------- +
| Log out       |
+ ------------- +
```

## A grid of blocks

```
+ ----------------------------------------------- +
| JP1269320                                       |
+ ---------------- + ------------ + ------------- +
|                  | Name:        | Hei Yiu Tsang |
|                  + ------------ + ------------- +
|                  | Age:         | 24            |
|                  + ------------ + ------------- +
| Photo            | Gender:      | Male          |
|                  + ------------ + ------------- +
|                  | Nationality: | Hong Kong     |
|                  + ------------ + ------------- +
|                  | Expiration:  | 01/12/2029    |
+ ---------------- + ------------ + ------------- +
```

## Tags

You can put information at the top of every block

```
+ icon ------------- +
|                    |
| I am a lonely icon |
|                    |
+ ------------------ +
```

## Nested blocks

```
+ blog page ----------------------------------------------------------- +
| + side bar ---------- + top bar ----------------------------------- + |
| |                     | Anthony's Fever dream + search bar ------ + | |
| |                     |                       | Search topic      | | |
| |                     |                       + ----------------- + | |
| |                     + ------------------------------------------- + |
| |                     | This is a collection of fever dreams.       | |
| |                     + blog posts -------------------------------- + |
| | + about button -- + |                                             | |
| | | About           | |                                             | |
| | + project button  + |                                             | |
| | | Project         | |                                             | |
| | + blog button --- + |                                             | |
| | | Blogs           | |                                             | |
| | + --------------- + |                                             | |
| |                     |                                             | |
| |                     |                                             | |
| |                     |                                             | |
| + ------------------- + ------------------------------------------- + |
+ --------------------------------------------------------------------- +
```

## Details

You can add details to your drawing! Syntax is always <subject> <verb> <object>

```
side bar is 20% parent.
top bar fills the rest.

about button's column is centered vertically.
```

### Subject

```
side bar          // just a block
side bar, top bar // multiple blocks
side bar's column // the container of the block
side bar's parent // the parent of the block
side bar's children // all direct children of the block
side bar's #1 child // first direct child of the block
```

### Units

```
10 px
20% parent
3 font height
centered vertically
```

## Mimified version (not sure)

```
+ #blog page
+ #side bar --------- + #top bar
|                     | Anthony's Fever dream + #search bar ----- +
|                     |                       | Search topic      |
|                     |                       + ----------------- +
|                     +
|                     | This is a collection of fever dreams.
|                     + #blog posts
| + #about button     |
| | About             |
| + #project button   |
| | Project           |
| + #blog button --   |
| | Blogs             |
| + ---------------   |
|                     |
|                     |
|                     |
+ ------------------- +
```
