document.addEventListener('DOMContentLoaded', () => {
  let currentCard = 0;

  const cards = document.querySelectorAll('.index-card');
  const outputBox = document.getElementById('output-card');

  function showCard(index) {
    cards.forEach((card, i) => {
      card.style.display = i === index ? 'block' : 'none';
    });
    // Hide output card unless we’re on it
    if (index !== cards.length - 1 && outputBox) {
      outputBox.style.display = 'none';
    }
  }

  function getInputValues() {
    return {
      first: document.getElementById('first')?.value || '',
      middle: document.getElementById('middle')?.value || '',
      last: document.getElementById('last')?.value || '',
      alias: document.getElementById('alias')?.value || '',
      email: document.getElementById('email')?.value || '',
      location: document.getElementById('location')?.value || '',
      phone: document.getElementById('phone')?.value || ''
    };
  }

  function showSpinner(show) {
    let spinner = document.getElementById('spinner');
    if (!spinner) {
      spinner = document.createElement('div');
      spinner.id = 'spinner';
      spinner.textContent = '🔄 Scanning...';
      spinner.style.cssText = 'margin-top:1em; font-weight:bold;';
      document.querySelector('.container').appendChild(spinner);
    }
    spinner.style.display = show ? 'block' : 'none';
  }

  // Safe ccall wrapper (module names must match your wrapped globals)
  function ccallSafe(mod, fname, rettype, argtypes, args) {
    if (!mod || typeof mod.ccall !== 'function') {
      return `⚠️ Module not loaded for ${fname}.`;
    }
    try {
      return mod.ccall(fname, rettype, argtypes, args);
    } catch (e) {
      return `❌ Error calling ${fname}: ${e.message}`;
    }
  }

  function runOSINT() {
    const input = getInputValues();
    return ccallSafe(
      window.OsintModule,
      'sweep_osint',
      'string',
      ['string', 'string', 'string', 'string', 'string', 'string'],
      [input.first, input.middle, input.last, input.alias, input.email, input.location]
    );
  }

  function runRecords() {
    const input = getInputValues();
    const full = (input.first + ' ' + input.last).trim();
    return ccallSafe(window.RecordsModule, 'search_records', 'string', ['string'], [full]);
  }

  function runAvatarLookup() {
    const input = getInputValues();
    return ccallSafe(window.AvatarLookupModule, 'avatar_lookup', 'string', ['string'], [input.email]);
  }

  function runFriendMap() {
    const input = getInputValues();
    return ccallSafe(window.FriendMapModule, 'friend_map', 'string', ['string', 'string'], [input.alias, input.email]);
  }

  function runReverseLookup() {
    const input = getInputValues();
    return ccallSafe(window.ReverseLookupModule, 'reverse_lookup', 'string', ['string', 'string', 'string'], [input.email, input.alias, input.location]);
  }

  function runSearchEngine() {
    const input = getInputValues();
    const full = (input.first + ' ' + input.last).trim();
    return ccallSafe(window.SearchModule, 'search_engines', 'string', ['string', 'string'], [full, input.alias]);
  }

  function runFullScan() {
    showSpinner(true);
    setTimeout(() => {
      let result = '=== HONEYGRABBER FULL SCAN ===\n';
      try {
        result += '\n--- OSINT ---\n' + runOSINT();
        result += '\n--- RECORDS ---\n' + runRecords();
        result += '\n--- AVATAR ---\n' + runAvatarLookup();
        result += '\n--- FRIEND MAP ---\n' + runFriendMap();
        result += '\n--- REVERSE LOOKUP ---\n' + runReverseLookup();
        result += '\n--- SEARCH ENGINE ---\n' + runSearchEngine();
      } catch (e) {
        result += '\n❌ Error occurred during scan: ' + e.message;
      }
      const out = document.getElementById('output');
      if (out) out.value = result;
      if (outputBox) outputBox.style.display = 'block';
      showSpinner(false);
      showCard(cards.length - 1); // show results card
    }, 100);
  }

  // Event delegation for buttons inside cards
  document.addEventListener('click', (e) => {
    if (e.target.closest('.next-btn')) {
      // last input card is second-to-last (last is results)
      const lastInputCardIndex = cards.length - 2;
      if (currentCard < lastInputCardIndex) {
        currentCard++;
        showCard(currentCard);
      } else if (currentCard === lastInputCardIndex) {
        runFullScan();
      }
    } else if (e.target.closest('.back-btn')) {
      if (currentCard > 0) {
        currentCard--;
        showCard(currentCard);
      }
    }
  });

  showCard(0);
});
