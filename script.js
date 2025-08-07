// Collects form input values
function getInputValues() {
  return {
    first: document.getElementById('first').value,
    middle: document.getElementById('middle').value,
    last: document.getElementById('last').value,
    alias: document.getElementById('alias').value,
    email: document.getElementById('email').value,
    location: document.getElementById('location').value
  };
}

function showSpinner(show) {
  let spinner = document.getElementById('spinner');
  if (!spinner) {
    spinner = document.createElement('div');
    spinner.id = 'spinner';
    spinner.innerHTML = '🔄 Scanning...';
    spinner.style.cssText = 'margin-top:1em; font-weight:bold;';
    document.querySelector('.container').appendChild(spinner);
  }
  spinner.style.display = show ? 'block' : 'none';
}

function runOSINT() {
  const input = getInputValues();
  return Module.ccall(
    'sweep_osint', 'string',
    ['string', 'string', 'string', 'string', 'string', 'string'],
    [input.first, input.middle, input.last, input.alias, input.email, input.location]
  );
}

function runRecords() {
  const input = getInputValues();
  return Module.ccall(
    'search_records', 'string', ['string'],
    [input.first + ' ' + input.last]
  );
}

function runAvatarLookup() {
  const input = getInputValues();
  return Module.ccall(
    'avatar_lookup', 'string', ['string'], [input.email]
  );
}

function runFriendMap() {
  const input = getInputValues();
  return Module.ccall(
    'map_friends', 'string', ['string', 'string'], [input.alias, input.email]
  );
}

function runReverseLookup() {
  const input = getInputValues();
  return Module.ccall(
    'reverse_lookup', 'string', ['string', 'string', 'string'],
    [input.email, input.alias, input.location]
  );
}

function runSearchEngine() {
  const input = getInputValues();
  return Module.ccall(
    'search_engine_dork', 'string', ['string', 'string'],
    [input.first + ' ' + input.last, input.alias]
  );
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
    document.getElementById('output').value = result;
    showSpinner(false);
  }, 100);
}

