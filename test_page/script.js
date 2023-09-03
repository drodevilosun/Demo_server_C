const $ = document.querySelector.bind(document);

const app = (()=> {
    const submit = $("#submit");
    const progressBar = $("#progress-bar");
    const responseCount = $("#response-count");
    
    return {
        init() {
            const numRequests = 10;
            submit.onclick = ()=>{
                let countRes = 0;
                console.log("clinking");
                // percentage of success response
                const percent = 100 / numRequests;
                for (let i = 0; i < numRequests; i++) {
                    fetch('http://localhost:8989', { method: 'GET' })
                        .then((response) => {
                            if (response.status == 200) {
                                countRes++;
                                console.log(`Request ${i + 1} succeeded`);
                            }
                            else
                                console.log(`Request ${i + 1} failed, status code: ${response.status}`);
                            
                            // Update progress bar
                            let completed = countRes + percent;
                            progressBar.style.width = `${countRes * percent}%`;
                            responseCount.innerHTML = `Responses: ${countRes}`;

                            if (countRes === numRequests) {
                                console.log("All requests completed");
                            }
                        })
                        .catch((error) => {
                            console.error(`Request ${i+1} error: ${error.message}`);
                            countRes++;
                            progressBar.style.width = `${countRes * percent}%`;
                            responseCount.innerHTML = `Responses: ${countRes}`;
                            if (countRes === numRequests) {
                                console.log("All requests completed");
                            }
                        });
                }
            }
            console.log("haha");
        }
    }
})();

app.init();