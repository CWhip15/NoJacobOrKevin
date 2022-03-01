 // Setting up the variables for converting from millitary time to standard AND adjusting the time-zone
 // String amPM = " XX";
            // String currHours = "XX";
            // String currMinutes = "XX";
            
            // // The place in the US where we are getting our time is 5 hours ahead of us, so we adjust.
            // if ((timeClient.getHours() - 5) < 0) { // If current hours roll into the negatives, adjust to be before 12 AM
            //     currHours = String(timeClient.getHours() + 8);
            //     amPM = " PM";
            // } else if ((timeClient.getHours() - 5) > 12) { // 1 PM - 11 PM case
            //     currHours = String(timeClient.getHours() - 12);
            //     amPM = " PM";
            // } else if ((timeClient.getHours() - 5) == 12) { // 12 PM case (Mid-day)
            //     currHours = String(timeClient.getHours() - 5);
            //     amPM = " PM";
            // } else if ((timeClient.getHours() - 5) == 0) { // 12 AM case (Mid-night)
            //     currHours = String(timeClient.getHours() + 12);
            //     amPM = " AM";
            // } else { // 1 AM - 11 AM case
            //     currHours = String(timeClient.getHours() - 5);
            //     amPM = " AM";
            // }

            // // The place in the US where we are getting our time is 22 minutes ahead of us, so we adjust.
            // // Make sure that the minutes don't underflow
            // if ((timeClient.getMinutes() - 22) < 0) {

            //     // Checking to see if minutes are < 10 so that we can add a 0 before the single-digit number
            //     if ((timeClient.getMinutes() - 22) < 10 || (timeClient.getMinutes() - 22) == 0) {
            //         currMinutes = String("0" + timeClient.getMinutes() + 38);
            //     } else {
            //         currMinutes = String(timeClient.getMinutes() + 38);
            //     }

            // } else {

            //     // Checking to see if minutes are < 10 so that we can add a 0 before the single-digit number
            //     if ((timeClient.getMinutes() - 22) < 10 || (timeClient.getMinutes() - 22) == 0) { 
            //         currMinutes = String("0" + timeClient.getMinutes() - 22);
            //     } else {
            //         currMinutes = String(timeClient.getMinutes() - 22);
            //     }
            // }

            // // Create a string which contains all of the calculated values
            // String currentTime = currHours + ":" + currMinutes + amPM;

            // // Print the string with all time-based components
            // M5.Lcd.printf(currentTime.c_str());