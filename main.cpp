#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "datastructures.h"
#include "cell.h"
#include "piece.h"
#include "board.h"

Board CheckersBoard;

int main(void)
{  
   initwindow(1200, 900);
   
   //Draws the initial state of board
   DrawBoard( &CheckersBoard );
   
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   
   outtextxy(700, 90,"No Piece Selected");
   
   int turn = RED;
   
   int mouseX, mouseY, selectionChanged = FALSE;
   
   char * turnColor  = (char *)malloc( 5 * sizeof(char) );
   
   strcpy(turnColor, "RED");
   
   while(true)
   {
       DrawIndicator(&CheckersBoard);
       
       //turn indicator
       strcmp(turnColor, "BLUE") ? outtextxy(600, 110, "RED's turn") : outtextxy(600, 110,"BLUE's turn");
       
       //check if correct piece is selected
       if ( !selectionChanged )
       {
           //selectionChanged = FALSE;
           getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
       }
       // if the button was clicked, then we check if the correct piece was selected
       if ( !( mouseX == -1 && mouseY == -1 ) )
       {
           //for ex, it this is blue's turn but user clicks on a red piece
           //this check tests that scenario
           if ( getpixel(mouseX, mouseY) != turn )
           {
               outtextxy(600, 90,"Select correct piece");
               
               if ( turn == RED )
                   outtextxy(600, 110,"Select RED colored piece");
               else 
                   outtextxy(600, 110,"Select BLUE colored piece");    
           }
           else
           {              
               //user has selected the correct piece, now we have to identify the possible targets for the move
               PtrCell clickedCell = GetClickedCell( mouseX, mouseY, &CheckersBoard );
               
               PtrCell target1 = NULL, target2 = NULL; //represent cells with jumped over pieces in case of a jump, empty highlighted cells otherwise
               PtrCell jumpedCell1 = NULL, jumpedCell2 = NULL; // represent targets in case of jump, NULL otherwise
               
               //jumpedCell1 = jumpedCell2 = GetCellByRowColumn(0,0, &CheckersBoard, FALSE, turn, FALSE);
               
               if ( !IdentifyAndHighlightTargets(turn, clickedCell, &target1, &target2, &jumpedCell1 , &jumpedCell2, &CheckersBoard ) )
               {
                   selectionChanged = FALSE;
                   continue;
               }
               //now, targets have been identified and highlighted
//                                             
//               PrintRC(target1, 650, 360);
//               PrintRC(target2, 650, 400);
//               
//               PrintRC(jumpedCell1, 650, 440);
//               PrintRC(jumpedCell2, 650, 480);
               
               //we need to intercept clicks on target
               
               //IDenftify which target was selected
               PtrCell clickedTarget = NULL;
               
               //this forces the user to select a valid target
               //until the the mouse is clicked, this loop will keep on polling the device
               int targetSelected = 0;
               
               while( ! ( targetSelected = InterceptTargetClicks(&clickedTarget, target1, target2, turn, &CheckersBoard, &mouseX, &mouseY) ) )
               {
                  //well, we can wait till the user selects a target
               } //end while for target selection
               
               //When we exit the above loop, clickedTarget contains the address of a valid target 
                
               //in case of jump move, we need to update the 'jumped over' cell data as well;
               
               if ( jumpedCell1 != NULL && targetSelected == TARGET_CLICK_1)
               {
                   DrawCell(jumpedCell1, jumpedCell1->Row, jumpedCell1->Column);
                   jumpedCell1->Piece->State = Removed;
                   jumpedCell1->Piece = NULL;
                   jumpedCell1->IsOccupied = FALSE;
                   jumpedCell1->OccupiedBy = 0;
               }
               
               if ( jumpedCell2 != NULL && targetSelected == TARGET_CLICK_2)
               {
                   DrawCell(jumpedCell2, jumpedCell2->Row, jumpedCell2->Column);
                   jumpedCell2->Piece->State = Removed;
                   jumpedCell2->Piece = NULL;
                   jumpedCell2->IsOccupied = FALSE;
                   jumpedCell2->OccupiedBy = 0;
               }
               
               //end jump specific code
               
               //now we have to move the piece to clicked cell
               
               if ( targetSelected != CHANGE_PIECE )
               {
                   MovePiece(&CheckersBoard, clickedCell, clickedTarget, target1, target2, turn );

                   //set values for next turn
                   turn = turn == BLUE ? RED : BLUE; 
                   strcmp(turnColor, "RED") ? strcpy(turnColor, "RED") : strcpy(turnColor, "BLUE") ;
                   
                   selectionChanged = FALSE;
                   
                   PlayAITurn(&CheckersBoard, turn);
                   
                   turn = turn == BLUE ? RED : BLUE; 
                   strcmp(turnColor, "RED") ? strcpy(turnColor, "RED") : strcpy(turnColor, "BLUE") ;                   
               }
               else
               {
                   selectionChanged = TRUE;
               }

           }
       }
   }

   closegraph();
      
   return 0;
}