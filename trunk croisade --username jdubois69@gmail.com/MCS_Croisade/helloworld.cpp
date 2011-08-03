//============================================================================
// Name        : helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <string>

#include "Dictionnary.h"

#include "SearchProcess.h"

#include <ctime>
#include <omp.h>
#include <mpi.h>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
	int nb_threads = 1;
	int wordsize = 4;
	int work_item_granularity = 5;
	int chunk_size = 1;
	bool print_results = false;

	string filename = "OGD_propres.txt";
	int c;
    while ((c = getopt (argc, argv, "d:t:w:g:c:p")) != -1)
    {
    	//cout << "Option : -" << (char)c << endl;
      switch (c)
        {
		  case 'd':
			filename = optarg;
			//cout << "dico = " << optarg << endl;
			break;
        case 't':
          nb_threads = atoi(optarg);
          //cout << "nbThreads = " << nb_threads << endl;
          break;

        case 'w':
          wordsize = atoi(optarg);
          break;

        case 'g':
        	work_item_granularity = atoi(optarg);
          break;

        case 'c':
        	chunk_size = atoi(optarg);
          break;
        case 'p':
        	print_results = true;
          break;

        case '?':
          if (optopt == 'c')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          else
            fprintf (stderr,
                     "Unknown option character `\\x%x'.\n",
                     optopt);
          return 1;
        default:
          abort ();
        }
    }


	MPI_Init(&argc, &argv);
	int my_rank = MPI::COMM_WORLD.Get_rank();
	int nb_procs = MPI::COMM_WORLD.Get_size();
	if (!my_rank)
	{
		cout << "Program is running with following parameters :" << endl;
		cout << "Dictionnary          : " << filename << endl;
		cout << "Wordsize             : " << wordsize << endl;
		cout << "Nb threads           : " << nb_threads << endl;
		cout << "Nb procs MPI         : " << nb_procs << endl;
		cout << "Workitem granularity : " << work_item_granularity << endl;
	}
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	//string filename = "OGD_communs.txt";

	//for (int wordsize = 6; wordsize >= 6 ; wordsize--)
	//{
		MPI::COMM_WORLD.Barrier();


		omp_set_num_threads(nb_threads);
		Dictionnary dico(filename,wordsize);
		cout << "Nb words in dico : " << dico.wordsNumber_ << endl;
	//	MPI::Finalize();
		//exit(0);
		//dico.str();
		double starting_clock = MPI::Wtime();
		//TO DO :
		/*
		 * WORKITEMS + WORKMANAGER POUR LES GERER
		 * BOUCLER SUR LES WORKITEMS DU WORKMANAGER AVEC BOUCLE OMP
		 */
		vector<Workitem> workItems;
		int workload = 0;
		while(workload + work_item_granularity < dico.wordsNumber_)
		{
			int idxBeg = workload;
			int idxEnd = workload + work_item_granularity;
			int possibilities = 0;
			for (int k = idxBeg ; k < idxEnd ; k++)
			{
				const string & word = dico.words_[k];
				for (int kk = 0; kk< wordsize ; kk++)
				{
					const Tree * t  = dico.tree.get_node(word[kk]);
					if (t)
						possibilities += t->nb_words_sub_tree / (double)(kk+1);
					else
						possibilities = 0;
				}
			}
			workItems.push_back(Workitem(idxBeg,idxEnd,possibilities));
			workload += work_item_granularity;
			//cout << "possibilities for workitem " << idxBeg << " = " << possibilities << endl;
		}

		{
			int work_rest = dico.wordsNumber_ - workload;
			int idxBeg = workload;
			int idxEnd = workload + work_rest;
			int possibilities = 0;
			for (int k = idxBeg ; k < idxEnd ; k++)
			{
				possibilities += dico.tree.get_node(dico.words_[k][0])->nb_words_sub_tree;
			}
			workItems.push_back(Workitem(idxBeg,idxEnd,possibilities));
		}
		vector<Workitem> my_workitems;
		std::sort(workItems.begin(),workItems.end());
		std::reverse(workItems.begin(),workItems.end());

		int wi = my_rank;
		int nb_words_in_my_wi = 0;
		for (; wi < workItems.size() ; wi+=nb_procs)
		{
			const Workitem & worki = workItems[wi];
			my_workitems.push_back(worki);
			nb_words_in_my_wi += (worki.idxEnd-worki.idxBeg);
		}
/*
		int rest = workItems.size() % nb_procs;
		if (rest)
			if (my_rank >= rest)
			{
				cout << my_rank << " putting " << wi << endl;
				const Workitem & worki = workItems[wi];
				my_workitems.push_back(worki);
				nb_words_in_my_wi += (worki.idxEnd-worki.idxBeg);
			}

		cout << my_rank << " has " << workItems.size() << " total workitems" << endl;
		cout << my_rank << " has " << my_workitems.size() << " workitems" << endl;
		MPI::Finalize();
		exit(0);
		*/
		cout << "First workitem = " << workItems[0].possibilities << endl;
		cout << "Last workitem = " << workItems[workItems.size()-1].possibilities << endl;
		int words_done = 0;
		double time_done = 0;
		int nb_grids_found = 0;

		/*
		int nb_wi = workItems.size() / nb_procs;
		myProcessDeb = my_rank * nb_wi;
		myProcessEnd = myProcessDeb + nb_wi;
		if (my_rank == nb_procs-1) myProcessEnd = workItems.size();
		cout << my_rank << " treating workitems " << myProcessDeb << " to " << myProcessEnd-1 << endl;
		*/
		#pragma omp parallel reduction(+ : nb_grids_found)
		{
			cout << "Thread working " << endl;
			SearchProcess sp(dico);
			sp.print_results = print_results;

			//cout << "Starting parallel resolution NOW!!!" << endl;
			#pragma omp for schedule(static,chunk_size)
			for(int i = 0 ; i < my_workitems.size() ; i++)
			{
				const Workitem & wi = my_workitems.at(i);
				//cout << "Workitem " << i << " = " << wi.idxBeg << " -> " << wi.idxEnd << endl;
				sp.run(wi,words_done,time_done);
				words_done += wi.idxEnd - wi.idxBeg;
				double end_time = MPI::Wtime();
				time_done = (end_time - starting_clock);
				double rest_time = (time_done / (double)words_done) * (nb_words_in_my_wi-words_done);
				cout << "[" << my_rank<<"] Done " << words_done << " / " << nb_words_in_my_wi << " in " << time_done << "secs. Remaining " << rest_time << " s" << endl;
			}
			//cout << "Found " << sp.nb_grids_found << " grids " << endl;
			nb_grids_found = sp.nb_grids_found;
		}//omlp parallel
		int glo_nb_grids_found = 0;
		MPI::COMM_WORLD.Allreduce(&nb_grids_found,&glo_nb_grids_found,1,MPI::INT,MPI::SUM);
		double ending_time = MPI::Wtime();
		double elapsed_time = (ending_time - starting_clock);
		if (!my_rank ) cout << "Found " << glo_nb_grids_found << " grids "<< wordsize << "x" << wordsize << " in " << elapsed_time << " secs" << endl;
	//}//wordsize
	MPI_Finalize();
	return 0;
}
