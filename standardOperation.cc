// Guidebook
// No two pieces can be captured at the one move
// create a map of legal moves and their capture

// King (v|h|d *1)
//      +1v || -1v || +1h || -1h
//      +1v+1h || +1v-1h || -1v+1h || -1v-1h
// *castling*

// Queen (v|h|d *n)
//      +nv || -nv || +nh || -nh
//      +nv+nh || +nv-nh || -nv+nh || -nv-nh

// Rook (v|h *n)
//      +nv || -nv || +nh || -nh
// *castling*

// Bishop (d *n)
//      +nv+nh || +nv-nh || -nv+nh || -nv-nh

// Knight 
//      +1v+2h || +2v+1h || -1v+2h || -2v+1h
//      +1v-2h || +2v-1h || -1v-2h || -2v-1h

// Pawn
// ** En passant must be available, or else -1v +1v can't do it!
// if (caputreCase: enemy piece +-1v+1h)
//      +1v+1h || -1v+1h
// else if (captureCase: -1v || +1v)
//      +1v+1h || -1v+1h    <- capture = Position pos
// else if (firstMove)
//      +1v || +2v
// else
//      +1v 
//      
// *promotion*

