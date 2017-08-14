/*************************************************************
**         Europäisches Institut für Systemsicherheit        *
**   Proktikum "Kryptoanalyse"                               *
**                                                           *
** Versuch 4: Kocher-Timing-Attack                           *
**                                                           *
**************************************************************
**
** attack.c: Timing-Attacke Rahmenprogramm
**/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <gmp.h>
#include "texp.h"

#include "daemon.h"


#define EXPBITS 128 /* Anzahl der Bits im Exponent */

//longnum n; /* Modulus */
//longnum y_trial; /* Hier soll der geheime Exponent y berechnet werden */
//longnum y_ok; /* Tatsaechlicher geheimer Exponent (vom Daemon) */*/
mpz_t n; /* Modulus */
mpz_t y_trial; /* Hier soll der geheime Exponent y berechnet werden */
mpz_t y_ok; /* Tatsaechlicher geheimer Exponent (vom Daemon) */
int ok; /* war der Exponent richtig? */

const unsigned long expected_timing = (1<<18);
/* Erwartungswert fuer den Zeitaufwand einer Multiplikation */


int main (void)
{
  connect_daemon (n); /* Mit dem Daemonen verbinden und den Modulus
                        * abholen */

  /* XXX Aufgabe: mit exp_daemon () Samples generieren und y_trial berechnen */
  mpz_t z, x0;
  mpz_init (z);
  mpz_init (x0);
  
  //berechne Hamming-Gewicht des Exponenten mit Basis (x) 1
   mpz_set_ui(x0, 1);

  // unsigned long LITTimeModSquare (const_longnum_ptr x, const_longnum_ptr n);
  unsigned long potTime =   EXPBITS * LITTimeModSquare(x0, n);
  
  // unsigned long LITTimeModMult (const_longnum_ptr x, const_longnum_ptr y, const_longnum_ptr n);
  unsigned long multTime = LITTimeModMult(x0, x0, n);
  unsigned long expTime = exp_daemon(z, x0);
  unsigned long hamWeight = (expTime - potTime) / multTime; 
  
   printf ("Hamming weight: %lu %lu %lu %lu \n", hamWeight, expTime, potTime, multTime);
  
  
  // unsigned long LITTimeModMult (const_longnum_ptr x, const_longnum_ptr y, const_longnum_ptr n);
   
  // unsigned long exp_daemon (longnum_ptr z, const_longnum_ptr x)
  
  unsigned long timings[100];
  mpz_t samples[100];
  
 /* for (unsigned int x = 0; x < 100; x++) {
	timings[x] = exp_daemon(x, samples[x]);
	
  }
  
  */

  // printf ("Berechneter Exponent: %s\n", LLong2Hex (&y_trial, 0, 1, 1));
	printf ("Berechneter Exponent: %s\n", mpz_get_str(NULL, 16, y_trial));

  ok = key_daemon (y_ok, y_trial);
  printf ("Das war %s\n", ok?"richtig":"falsch");
  if (! ok) {
    // printf ("Richtig war: %s\n", LLong2Hex (&y_ok, 0, 1, 1));
		printf ("Richtig war: %s\n", mpz_get_str(NULL, 16, y_ok));
  }
  disc_daemon (); /* Verbindung zum Daemon beenden */
  exit (0);
}
